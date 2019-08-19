#ifndef _DISPLAY_H
#define _DISPLAY_H
#include <String.h>
#include <WString.h>
#include <Adafruit_NeoPixel.h>
#include <DS3231.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
#include <DHT.h>

class Display
{
    private:
        enum Mode {LET=1,NUM,CUS};
        enum Letters {SMALL, BIG};
        String path;
        String text;
        int num;
        Mode mode;
        Letters font_s;
        bool fopen;
        int x_pos;
        int y_pos;
        int tab_all[50];
        int * tab_custom;
        int size;
        int r,g,b;
        int *pt_x;
        int *pt_y;
        int how_many;
        int r_bg,g_bg,b_bg;
        Adafruit_NeoPixel strip;
        void SD_reader();
        void display_pixels();
    public:
        Display();
        Display(int * tab, int sof);
        void show(const String text);
        void show(int no);
        void show();
        void begin();
        //void no_separator(const int & no);
        void set_position(int x, int y);
        void set_text_colors(int rc, int gc, int bc);
        void set_bg_colors(int rc, int gc, int bc);
        void erase_it();
        void font_size(int size);
        void clear_all();
};


class Clock
{
    private:
        enum DotsOn {OFF, ON};
        DotsOn mode = OFF;
        DS3231 rtc;
        Time t;
        Display secs[2];
        Display mins[2];
        Display hour[2];
        const int y = 10;
        int dots_array[8] = {12+(y+2)*40, 12+(y+3)*40, 12+(y+5)*40, 12+(y+6)*40,
                             26+(y+2)*40, 26+(y+3)*40, 26+(y+5)*40, 26+(y+6)*40};
        Display dot;
        int *pd;
        int i = 0;
        int t_s1 = -1;
        int t_s2 = -1;
        int t_min1 = -1;
        int t_min2 = -1;
        int t_hour1 = -1;
        int t_hour2 = -1;
        bool both_sec = false;
        bool both_min = false;
        bool both_hour = false;
        bool first_run = true;
        int * no_separator(const int & no);
        void seconds();
        void minutes();
        void hours();
    public:
        Clock();
        void show_time();
        void begin();
        void stop();
};

class DHTsensor
{
    private:
        DHT dht;
        Display tempdis[2];
        Display tmark;
        Display hmark;
        Display humdis[2];
        Display bgcol;
        bool both_temp = false;
        bool both_hum = false;
        int t1 = -1;
        int t2 = -1;
        int h1 = -1;
        int h2 = -1;
        int *pd;
        float temperature;
        float humidity;
        void read();
        int * no_separator(const int & no);
        void show_temp();
        void show_hum();
    public:
        void begin();
        DHTsensor();
        void show_dht();
};
#endif // DISPLAY_H
