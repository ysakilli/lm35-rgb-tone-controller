#include <16f877a.h>

#device ADC=10 

#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD

#use delay (clock=4000000)

#define preload 65458
#define red pin_e0
#define green pin_e1
#define blue pin_e2
#define use_portb_lcd TRUE  

#include <lcd.c>  
#priority timer2,timer1

unsigned long int bilgi;
float sicaklik;

int8 red_duty;
int8 green_duty;
int8 blue_duty;
int sayici;

void adc()
{
      bilgi=read_adc(); 
      sicaklik=(0.0048828125*bilgi)*100; //sýcaklýk hesaplama
}



void lcd()
{
      lcd_gotoxy(10,1);
      printf(lcd_putc,"%3.0f'C",sicaklik);
      delay_ms(20);
      lcd_gotoxy(1,2);
      printf(lcd_putc,"\nSERKN-ARIF-YUSUF");
}


#INT_TIMER1
void timer()
{
    set_timer1(preload);
   
   if (sayici < 255)
    {
      if (sayici == red_duty)   output_low(red);
     
      if (sayici == green_duty) output_low(green);

      if (sayici == blue_duty)  output_low(blue);

    }
  
   else if(sayici == 255)
    {
      sayici = 0;
      if (red_duty==0) output_low(red);
      else output_high(red);
      if (green_duty==0) output_low(green);
      else output_high(green);
      if (blue_duty==0) output_low(blue);
      else output_high(blue);
    }
   sayici++;
}

void rgb ()
{
   if(sicaklik<0){red_duty=255; green_duty=255;   blue_duty=255;} //beyaz
   
   if(sicaklik>=0 && sicaklik<2){red_duty=0;   green_duty=255; blue_duty=51;}
   if(sicaklik>=2 && sicaklik<4){red_duty=0;   green_duty=255; blue_duty=102;}
    if(sicaklik>=4 && sicaklik<6){red_duty=0;   green_duty=255; blue_duty=153;}
   if(sicaklik>=6 && sicaklik<8){red_duty=0;   green_duty=255; blue_duty=204;}
   if(sicaklik>=8 && sicaklik<10){red_duty=0;   green_duty=255; blue_duty=255;} // turkuaz
   if(sicaklik>=10 && sicaklik<12){red_duty=0;   green_duty=204; blue_duty=255;}   
   if(sicaklik>=12 && sicaklik<14){red_duty=0;   green_duty=153; blue_duty=255;}
   if(sicaklik>=14 && sicaklik<16){red_duty=0;   green_duty=102; blue_duty=255;} 
   if(sicaklik>=16 && sicaklik<18){red_duty=0;   green_duty=51;  blue_duty=255;}
   if(sicaklik>=18 && sicaklik<20){red_duty=0;   green_duty=0;   blue_duty=255;} // mavi
   if(sicaklik>=20 && sicaklik<22){red_duty=51;  green_duty=0;   blue_duty=255;}
   if(sicaklik>=22 && sicaklik<24){red_duty=102; green_duty=0;   blue_duty=255;}
   if(sicaklik>=24 && sicaklik<26){red_duty=153; green_duty=0;   blue_duty=255;}   
   if(sicaklik>=26 && sicaklik<28){red_duty=204; green_duty=0;   blue_duty=255;}
   if(sicaklik>=28 && sicaklik<30){red_duty=255; green_duty=0;   blue_duty=255;} // pembe

   if(sicaklik>=30 && sicaklik<32){red_duty=255; green_duty=51;  blue_duty=0;}
   if(sicaklik>=32 && sicaklik<34){red_duty=255; green_duty=102; blue_duty=0;}
   if(sicaklik>=34 && sicaklik<36){red_duty=255; green_duty=153; blue_duty=0;}
   if(sicaklik>=36 && sicaklik<38){red_duty=255; green_duty=204; blue_duty=0;}
   if(sicaklik>=38 && sicaklik<40){red_duty=255; green_duty=255; blue_duty=0;} // sari

   
   if(sicaklik>=40 && sicaklik<42){red_duty=51; green_duty=0;   blue_duty=0;}
   if(sicaklik>=42 && sicaklik<44){red_duty=102; green_duty=0;   blue_duty=0;}
   if(sicaklik>=44 && sicaklik<46){red_duty=153; green_duty=0;   blue_duty=0;}
   if(sicaklik>=46 && sicaklik<48){red_duty=204; green_duty=0;   blue_duty=0;}
   if(sicaklik>=48){red_duty=255; green_duty=0;  blue_duty=0;}// kirmizi
   sayici = 0;
}

void main()
{
   setup_psp(PSP_DISABLED);     
  
  
   setup_timer_2(T2_DIV_BY_4,255,1);
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
  
   set_tris_a(0x01);
   
   set_tris_e(0b000);
     
   setup_adc(adc_clock_internal);
   setup_adc_ports(AN0);
  
   lcd_init();  

   set_adc_channel(0); 
   delay_us(20);        

   printf(lcd_putc,"\fSICAKLIK");
    
  
   for(;;) 
   {
     
      adc();         
      lcd();
     
      rgb();
   }
   
}

