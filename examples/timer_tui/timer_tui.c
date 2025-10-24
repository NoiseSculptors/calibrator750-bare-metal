
#include "gpio.h"
#include "init.h"
#include "printf.h"
#include "rcc.h"
#include "timer.h"
#include "serial.h"
#include "usart_uart_lpuart.h"
#include <stdint.h>
#include <stdbool.h>

/* Output on PB13 */

static int uart_getch_nb(void)
{
    if (((*USART1_ISR) & (1u<<5)) == 0) return -1; /* RXNE */
    return (int)(*USART1_RDR & 0xFF);
}

/* ------------------- ANSI helpers ------------------- */
static inline void ansi_cls(void){ printf("\x1b[2J\x1b[H"); }
static inline void ansi_home(void){ printf("\x1b[H"); }
static inline void ansi_hide(void){ printf("\x1b[?25l"); }
static inline void ansi_show(void){ printf("\x1b[?25h"); }
static inline void ansi_rev_on(void){ printf("\x1b[7m"); }
static inline void ansi_rev_off(void){ printf("\x1b[27m"); }

/* ------------------- Key decoding ------------------- */
typedef enum { K_NONE=0, K_TAB, K_STAB, K_UP, K_DOWN, K_PGUP, K_PGDN, K_LEFT, K_RIGHT, K_CHAR } key_t;

static key_t read_key(int *out_ch)
{
    int c = uart_getch_nb();
    if (c < 0) return K_NONE;

    if (c == '\t') return K_TAB;
    if (c == 0x1B) { /* ESC */
        int c1; while((c1=uart_getch_nb())<0){} /* expect '[' or 'O' */
        if (c1 == '[') {
            int c2; while((c2=uart_getch_nb())<0){}
            switch(c2){
                case 'A': return K_UP;
                case 'B': return K_DOWN;
                case 'C': return K_RIGHT;
                case 'D': return K_LEFT;
                case 'Z': return K_STAB; /* some terms: Shift+TAB */
                case '5': { int t; while((t=uart_getch_nb())<0){} (void)t; return K_PGUP; } /* ~ */
                case '6': { int t; while((t=uart_getch_nb())<0){} (void)t; return K_PGDN; } /* ~ */
            }
        }
        return K_NONE;
    }
    *out_ch = c;
    return K_CHAR;
}

/* ------------------- UI model ------------------- */
typedef struct {
    uint32_t f_tim_hz; /* timer kernel clock (user-supplied) */
    uint32_t psc;
    uint32_t arr;
    uint32_t ccr1;
    uint8_t  oc1m;     /* 0..7; we use 6=PWM1, 7=PWM2, 3=Toggle */
    uint8_t  cc1e;     /* PA8 enable */
    uint8_t  cc1ne;    /* PA7 enable */
    uint8_t  pol_inv;  /* 0 normal, 1 inverted */
    uint8_t  moe;      /* main output enable */
    uint8_t  sel;      /* selected field index */
    uint8_t  ospeedr;
    uint32_t sysclk;
    bool     editing;
    char     editbuf[16];
    uint8_t  editlen;
} ui_t;

static ui_t ui;

static inline void field_bounds(uint8_t idx, uint32_t *minv, uint32_t *maxv, uint32_t *step, uint32_t *big)
{
    switch(idx){
    case 0: *minv=1000000u;    *maxv=600000000u; *step=100000u; *big=1000000u; break; /* f_tim */
    case 1: *minv=0u;          *maxv=0xFFFFu;    *step=1u;     *big=10u;      break; /* PSC */
    case 2: *minv=1u;          *maxv=0xFFFFu;    *step=1u;     *big=10u;      break; /* ARR */
    case 3: *minv=0u;          *maxv=0xFFFFu;    *step=1u;     *big=10u;      break; /* CCR1 */
    case 4: *minv=0u;          *maxv=7u;         *step=1u;     *big=1u;       break; /* OC1M */
    case 5: *minv=0u;          *maxv=1u;         *step=1u;     *big=1u;       break; /* CC1E */
    case 6: *minv=0u;          *maxv=1u;         *step=1u;     *big=1u;       break; /* CC1NE */
    case 7: *minv=0u;          *maxv=1u;         *step=1u;     *big=1u;       break; /* POL */
    case 8: *minv=0u;          *maxv=1u;         *step=1u;     *big=1u;       break; /* MOE */
    case 9: *minv=0u;          *maxv=3u;         *step=1u;     *big=1u;       break; /* OSPEEDR */
    default:*minv=0;           *maxv=0;          *step=0;      *big=0;        break;
    }
}

static inline uint32_t clampu(uint32_t v, uint32_t lo, uint32_t hi){ return (v<lo)?lo:((v>hi)?hi:v); }

/* integer compute for f_out (Hz) and duty (%) */
static void compute(uint32_t *fout_hz, uint32_t *duty_pct)
{
    uint64_t ft = (uint64_t)ui.f_tim_hz / (uint64_t)(ui.psc + 1u);
    uint64_t n  = (uint64_t)(ui.arr + 1u);
    uint32_t f  = (n ? (uint32_t)(ft / n) : 0u);
    uint32_t d  = (ui.arr + 1u) ? (uint32_t)((ui.ccr1 * 100u) / (ui.arr + 1u)) : 0u;
    *fout_hz = f;
    *duty_pct = d;
}

/* ------------------- Apply ------------------- */
static void apply_changes(void)
{
    /* GPIO: PB13 AF1 TIM1_CH1N */
    *RCC_AHB4ENR |= (1u << GPIOBEN);
    *GPIOB_MODER   &= ~(0x3u << (MODER13));
    *GPIOB_MODER   |=  (0x2u << (MODER13)); /* AF */
    *GPIOB_OSPEEDR &= ~(0x3u << (OSPEED13));
    *GPIOB_OSPEEDR |=  (ui.ospeedr << (OSPEED13));
    *GPIOB_PUPDR   &= ~(0x3u << (PUPDR13));
    *GPIOB_AFRH    &= ~(0xFu << (AFR13));
    *GPIOB_AFRH    |=  (0x1u << (AFR13)); /* AF1 */

    /* TIM1 clock on (APB2) */
    *RCC_APB2ENR |= (1u << 0); /* TIM1EN */

    /* Stop counter */
    *TIM1_CR1 &= ~(1u << 0);

    /* Base */
    *TIM1_PSC  = ui.psc;
    *TIM1_ARR  = ui.arr;
    *TIM1_CCR1 = ui.ccr1;

    /* OC1 mode + preload */
    *TIM1_CCMR1 &= ~(0x7u << 4);
    *TIM1_CCMR1 |=  ((ui.oc1m & 7u) << 4);
    *TIM1_CCMR1 |=  (1u << 3); /* OC1PE */

    /* Polarity + enables */
    if (ui.pol_inv) *TIM1_CCER |=  (1u << 1); else *TIM1_CCER &= ~(1u << 1);
    if (ui.cc1e)    *TIM1_CCER |=  (1u << 0); else *TIM1_CCER &= ~(1u << 0);
    if (ui.cc1ne)   *TIM1_CCER |=  (1u << 2); else *TIM1_CCER &= ~(1u << 2);

    /* Latch */
    *TIM1_EGR |= (1u << 0);   /* UG */

    /* MOE (advanced timer) */
    if (ui.moe) *TIM1_BDTR |=  (1u << 15); else *TIM1_BDTR &= ~(1u << 15);

    /* Go */
    *TIM1_CR1 |= (1u << 0);
}

/* ------------------- Render UI ------------------- */
static void render(void)
{
    static const char *modes[8] = {"Froz","ActLo","Inactive","Toggle","ForceLo","ForceHi","PWM1","PWM2"};
    uint32_t f, d; compute(&f, &d);

    ansi_cls();
    printf("STM32H7 TIM1 live console  |  Left/Right nav  |  Up/Down +/-  |  PgUp/PgDn big  |  e=edit  r=apply  q=quit\n\n");
    printf(" SYSCLK     : %d\n-----------------------\n", ui.sysclk);

    for (int i=0;i<10;i++){
        bool sel = (ui.sel == (uint8_t)i);
        if (sel) ansi_rev_on();
        switch(i){
            case 0: printf(" f_tim [Hz] : "); break;
            case 1: printf(" PSC        : "); break;
            case 2: printf(" ARR        : "); break;
            case 3: printf(" CCR1       : "); break;
            case 4: printf(" OC1M mode  : "); break;
            case 5: printf(" CC1E (PA8) : "); break;
            case 6: printf(" CC1NE(PA7) : "); break;
            case 7: printf(" POLarity   : "); break;
            case 8: printf(" MOE        : "); break;
            case 9: printf(" OSPEEDR    : "); break;
        }

        /* show edit buffer while editing the selected field */
        if (ui.editing && sel){
            printf("[%s]", ui.editbuf);
        } else {
            switch(i){
                case 0: printf("%u", ui.f_tim_hz); break;
                case 1: printf("%u", ui.psc);      break;
                case 2: printf("%u", ui.arr);      break;
                case 3: printf("%u", ui.ccr1);     break;
                case 4: printf("%s", modes[ui.oc1m & 7u]); break;
                case 5: printf("%s", ui.cc1e ? "ON" : "off"); break;
                case 6: printf("%s", ui.cc1ne? "ON" : "off"); break;
                case 7: printf("%s", ui.pol_inv? "INV":"norm"); break;
                case 8: printf("%s", ui.moe ? "ON" : "off"); break;
                case 9: printf("%u", ui.ospeedr);  break;
            }
        }
        if (sel) ansi_rev_off();
        printf("\n");
    }

    printf("\n Derived (only for PWM mode(s)): f_out ≈ %u Hz    duty ≈ %u %%\n", f, d);
}

/* ------------------- Edit helpers ------------------- */
static void start_edit(void)
{
    ui.editing = true;
    ui.editlen = 0;
    for (int i=0;i<(int)sizeof ui.editbuf; i++) ui.editbuf[i]=0;

    /* prefill with current */
    uint32_t cur=0;
    switch(ui.sel){
        case 0: cur=ui.f_tim_hz; break;
        case 1: cur=ui.psc;      break;
        case 2: cur=ui.arr;      break;
        case 3: cur=ui.ccr1;     break;
        case 4: cur=ui.oc1m;     break;
        case 5: cur=ui.cc1e;     break;
        case 6: cur=ui.cc1ne;    break;
        case 7: cur=ui.pol_inv;  break;
        case 8: cur=ui.moe;      break;
        case 9: cur=ui.ospeedr;  break;
    }
    /* integer to string via printf into buffer using xsprintf if you have it;
       otherwise simple manual fill: */
    /* quick u32->dec */
    char tmp[16]; int n=0; uint32_t v=cur;
    if (v==0){ ui.editbuf[0]='0'; ui.editbuf[1]=0; ui.editlen=1; return; }
    while(v){ tmp[n++] = (char)('0' + (v%10)); v/=10; }
    for (int i=0;i<n;i++) ui.editbuf[i]=tmp[n-1-i];
    ui.editbuf[n]=0; ui.editlen=(uint8_t)n;
}

static void finish_edit(bool apply_now)
{
    /* parse numeric, clamp */
    uint32_t v=0;
    for (uint8_t i=0;i<ui.editlen;i++){
        char c = ui.editbuf[i];
        if (c<'0' || c>'9') continue;
        v = v*10u + (uint32_t)(c - '0');
    }
    uint32_t minv,maxv,step,big; field_bounds(ui.sel,&minv,&maxv,&step,&big);
    v = clampu(v,minv,maxv);

    switch(ui.sel){
        case 0: ui.f_tim_hz = v; break;
        case 1: ui.psc      = v; break;
        case 2: ui.arr      = v; break;
        case 3: ui.ccr1     = v; break;
        case 4: ui.oc1m     = (uint8_t)v; break;
        case 5: ui.cc1e     = (uint8_t)(v?1:0); break;
        case 6: ui.cc1ne    = (uint8_t)(v?1:0); break;
        case 7: ui.pol_inv  = (uint8_t)(v?1:0); break;
        case 8: ui.moe      = (uint8_t)(v?1:0); break;
        case 9: ui.ospeedr  = v; break;
    }
    ui.editing = false;
    if (apply_now) apply_changes();
}

static void bump(int dir, bool bigstep)
{
    uint32_t minv,maxv,step,big; field_bounds(ui.sel,&minv,&maxv,&step,&big);
    uint32_t inc = bigstep ? big : step;

    uint32_t *p = NULL; uint8_t *pb=NULL;
    switch(ui.sel){
        case 0: p=&ui.f_tim_hz; break;
        case 1: p=&ui.psc; break;
        case 2: p=&ui.arr; break;
        case 3: p=&ui.ccr1; break;
        case 4: pb=&ui.oc1m; break;
        case 5: pb=&ui.cc1e; break;
        case 6: pb=&ui.cc1ne; break;
        case 7: pb=&ui.pol_inv; break;
        case 8: pb=&ui.moe; break;
        case 9: pb=&ui.ospeedr; break;
    }
    if (p){
        int64_t nv = (int64_t)(*p) + (dir>0 ? (int64_t)inc : -(int64_t)inc);
        if (nv < (int64_t)minv) nv = (int64_t)minv;
        if (nv > (int64_t)maxv) nv = (int64_t)maxv;
        *p = (uint32_t)nv;
    } else if (pb){
        int32_t nv = (int32_t)(*pb) + (dir>0 ? (int32_t)inc : -(int32_t)inc);
        if (nv < (int32_t)minv) nv = (int32_t)minv;
        if (nv > (int32_t)maxv) nv = (int32_t)maxv;
        *pb = (uint8_t)nv;
    }
}

/* ------------------- Main ------------------- */
int main(void)
{

    clock_info_t ci;
    ci = init_clock();
    init_serial(&ci, 115200);

    /* defaults */
    ui.f_tim_hz = ci.hclk_hz;
    ui.psc = 0;
    ui.arr = 23;
    ui.ccr1 = 12;
    ui.oc1m = 6;    /* PWM1 */
    ui.cc1e = 1;    /* PA8 on */
    ui.cc1ne= 1;    /* PA7 off by default */
    ui.pol_inv=0;
    ui.moe = 1;
    ui.sel = 2;     /* select ARR initially */
    ui.editing = false;
    ui.ospeedr = 0;
    ui.sysclk = ci.sysclk_hz; 

    ansi_hide();
    render();
    apply_changes();

    for(;;){
        int ch=0; key_t k = read_key(&ch);

        if (k == K_NONE) continue;

        if (ui.editing){
            if (k==K_CHAR && ch>='0' && ch<='9'){
                if (ui.editlen < sizeof(ui.editbuf)-1){
                    ui.editbuf[ui.editlen++] = (char)ch;
                    ui.editbuf[ui.editlen] = 0;
                }
                render();
            } else if (k==K_CHAR && (ch=='\r' || ch=='\n')){
                finish_edit(true);
                render();
            } else if (k==K_CHAR && (ch==27)){ /* ESC cancels */
                ui.editing=false;
                render();
            } else if (k==K_CHAR && (ch==8 || ch==127)){ /* backspace */
                if (ui.editlen>0){ ui.editlen--; ui.editbuf[ui.editlen]=0; }
                render();
            }
            continue;
        }

        switch(k){
            case K_RIGHT:ui.sel = (ui.sel+1u)%10u; render(); break;
            case K_LEFT: ui.sel = (ui.sel==0)?9:ui.sel-1; render(); break;
            case K_UP:   bump(+1,false); apply_changes(); render(); break;
            case K_DOWN: bump(-1,false); apply_changes(); render(); break;
            case K_PGUP: bump(+1,true); apply_changes();  render(); break;
            case K_PGDN: bump(-1,true); apply_changes();  render(); break;
            case K_CHAR:
                if (ch=='e'){ start_edit(); render(); }
                else if (ch=='r'){ apply_changes(); render(); }
                else if (ch=='q'){ ansi_show(); ansi_home(); while(1){} }
                break;
            default: break;
        }
    }
}

