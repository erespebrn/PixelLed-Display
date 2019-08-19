#include "Display.h"

#define NUMPIXELS 841
#define PIN 7
#define DHTPIN 2
#define DHTTYPE DHT22

extern int r_b;
extern int g_b;
extern int b_b;


Adafruit_NeoPixel st = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
DS3231 a(SDA,SCL);
Time temp;
DHT d(DHTPIN, DHTTYPE);

//Method to display numbers
void Display::show(int no)
{
    mode = NUM;
    num = no;
    int x_temp = x_pos;
    if(font_s == 0)
        path = "patterns/numbers/small/" + String(no) + ".txt";
    else if(font_s == 1)
        path = "patterns/numbers/" + String(no) + ".txt";
    SD_reader();
    display_pixels();
    strip.show();
    x_pos = x_temp;
}

void Display::show()
{
    display_pixels();
    strip.show();
}

void Display::erase_it()
{
    int r_temp = r;
    int g_temp = g;
    int b_temp = b;
    r=r_bg;
    g=g_bg;
    b=b_bg;
    if(mode == 1)
    {
        this->show(text);
    }
    else if(mode == 2)
    {
        this->show(num);
    }
    else
    {
        this->show();
    }


    r = r_temp;
    g = g_temp;
    b = b_temp;
}

//Method for displaying text
void Display::show(const String txt)
{
    mode = LET;
    text = txt;
    int z = text.length();
    int t = 0;
    int i = 0;
    String text_tab[z];
    Serial.println(z);
    int x_temp = x_pos;
    int y_temp = y_pos;


    for(i, t; i<z; i++, t++)
    {
        String a ="";
        char c = text.charAt(i);

        if(c == '%')
        {
            i++;
            int y = i;
            String mark = "";
            for(y; y<i+3; y++)
                mark += text.charAt(y);

            text_tab[t] = a+mark;
            i=y-1;
            continue;
        }

        text_tab[t] = a+c;
    }

    for(int i = 0; i <z; i++)
    {
        path = "patterns/letters/" + text_tab[i] + ".txt";
        Serial.println(text_tab[i]);
        SD_reader();
        display_pixels();

        if(text_tab[i] == "W" || text_tab[i] == "M" || text_tab[i] == "N" || text_tab[i] == "X" || text_tab[i] == "Y" || text_tab[i] == "Z" || text_tab[i] == "T")
            x_pos += 6;
        else if(text_tab[i] == "I")
            x_pos += 3;
        else if(text_tab[i] == "col")
            x_pos += 2;
        else
            x_pos += 5;
    }

    x_pos = x_temp;
    y_pos = y_temp;
    strip.show();
}

Display::Display()
{
    strip = st;
    x_pos = 1;
    y_pos = 1;
    g=20;
    r=60;
    b=20;
    r_bg = r_b;
    g_bg = g_b;
    b_bg = b_b;
}

Display::Display(int * tab, int sof)
{
    strip = st;
    x_pos = 1;
    y_pos = 1;
    g=20;
    r=60;
    b=20;
    r_bg = r_b;
    g_bg = g_b;
    b_bg = b_b;
    mode = CUS;
    tab_custom = tab;
    size = sof;
}

void Display::begin()
{
    Serial.begin(9600);
    strip.begin();
    Serial.print("Initializing SD card...");
    if (!SD.begin(4))
    {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");
}
//Method for digits/letters color setup
void Display::set_text_colors(int rc, int gc, int bc)
{
    rc = r;
    gc = g;
    bc = b;
}

//Method for background color setup
void Display::set_bg_colors(int rc, int gc, int bc)
{
    rc = r_bg;
    gc = g_bg;
    bc = b_bg;
}

//Method for position setup
void Display::set_position(int x, int y)
{
    x_pos = x;
    y_pos = y;
}

//Private method for SD Card reading - using SD.h class
void Display::SD_reader()
{
    File myFile;
    myFile = SD.open(path);
    fopen = myFile;

    if (fopen)
    {
        char inChar;
        char inData[3];
        int index;
        int i = 0;
        int tab_x[50];
        int tab_y[50];
        bool started = false;
        bool ended = false;
        bool isthisx = false;
        bool timefory = false;
        bool isthisy = false;

        while(myFile.available())
        {
            inChar = myFile.read();
            if (inChar == '<')
            {
                started = true;
                index = 0;
                inData[index] = '\0';
            }
            else if (inChar == '>')
            {
                ended = true;
            }
            else if (inChar == '&')
            {
                timefory = true;
                i = 0;
            }
            else if(started)
            {
                inData[index] = inChar;
                index++;
                inData[index] = '\0';
            }

            if(started && ended)
            {
                // Convert the string to an integer
                int inInt = atoi(inData);
                // Use the value
                if (!timefory)
                {
                    tab_x[i] = x_pos + inInt;
                    i++;
                }
                else
                {
                    tab_y[i] = y_pos + inInt;
                    i++;
                }
                // Get ready for the next time
                started = false;
                ended = false;
                isthisx = false;
                isthisy = false;
                index = 0;
                inData[index] = '\0';
            }
        }
        timefory = false;
        for(int h = 0; h<i; h++)
        {
            tab_all[h] = (tab_x[h]-1)+(tab_y[h]-1)*40;
        }
        how_many = i;
    }
    else
        Serial.print("Blad otwarcia pliku");

    myFile.close();

}

/*
void Display::no_separator(const int & no)
{
  help_tab[0] = 0;
  help_tab[1] = 0;
  int number = no;
  int i = 0;
  int cyfra = 0;
  int z;

  while(number>0)
  {
    cyfra = number % 10;
    help_tab[i] = cyfra;
    number /= 10;
    i++;
  }

  z = help_tab[0];
  help_tab[0] = help_tab[1];
  help_tab[1] = z;
}
*/
//Method for setting up pixels for next display.
void Display::display_pixels()
{
    int i = 0;
    if(mode != 3)
    {
        if(fopen)
        {
            while (i < how_many)
            {
                strip.setPixelColor(tab_all[i], strip.Color(g,r,b));
                i++;
            }
        }
    }
    else
    {
        while(i < size)
        {
            strip.setPixelColor(tab_custom[i], strip.Color(g,r,b));
            i++;
        }
    }
}

void Display::font_size(int size)
{
    if(size == 0)
        font_s = SMALL;
    else if(size == 1)
        font_s = BIG;
}

void Display::clear_all()
{
    for( int i = 0 ; i < NUMPIXELS; i++)
    {
         strip.setPixelColor(i, strip.Color(r_b,g_b,b_b));
    }
    strip.show();
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
Clock::Clock() : rtc(a), t(temp), dot(dots_array,8)
{

    secs[0].set_position(29, y);
    secs[1].set_position(35, y);
    secs[0].font_size(1);
    secs[1].font_size(1);
    mins[0].set_position(15, y);
    mins[1].set_position(21, y);
    mins[0].font_size(1);
    mins[1].font_size(1);
    hour[0].set_position(1, y);
    hour[1].set_position(7, y);
    hour[0].font_size(1);
    hour[1].font_size(1);
}

void Clock::begin()
{
    rtc.begin();
}

int * Clock::no_separator(const int & no)
{
    int tab_size = 0;
    if(no/10 == 0)
    {
        tab_size = 1;
    }
    else
    {
        tab_size = 2;
    }

    int *help_tab = new int[tab_size];
    help_tab[0] = 0;
    help_tab[1] = 0;
    int number = no;
    int i = 0;
    int cyfra = 0;
    int z;

    while(number>0)
    {
        cyfra = number % 10;
        help_tab[i] = cyfra;
        number /= 10;
        i++;
    }

    z = help_tab[0];
    help_tab[0] = help_tab[1];
    help_tab[1] = z;
    return help_tab;

}

void Clock::show_time()
{

    t = rtc.getTime();
    pd = no_separator(t.sec);
    seconds();
    pd = no_separator(t.min);
    minutes();
    pd = no_separator(t.hour);
    hours();
    first_run = false;
}

void Clock::seconds()
{
    if(t_s1 != pd[0] && t_s2 != pd[1])
    {
        secs[0].erase_it();
        secs[1].erase_it();
        secs[0].show(pd[0]);
        secs[1].show(pd[1]);

        if(mode == 0)
        {
            dot.erase_it();
            mode = ON;
        }
        else if(mode == 1)
        {
            dot.show();
            mode = OFF;
        }
        both_sec = true;
    }
    if(both_sec == false)
    {
        if(t_s2 != pd[1] || first_run == true)
        {
            secs[1].erase_it();
            secs[1].show(pd[1]);

            if(mode == 0)
            {
                dot.erase_it();
                mode = ON;
            }
            else if(mode == 1)
            {
                dot.show();
                mode = OFF;
            }
        }
    }

    t_s1 = pd[0];
    t_s2 = pd[1];
    both_sec = false;
    delete [] pd;
}

void Clock::minutes()
{
    if(t_min1 != pd[0] && t_min2 != pd[1])
    {
        mins[0].erase_it();
        mins[1].erase_it();
        mins[0].show(pd[0]);
        mins[1].show(pd[1]);
        both_min = true;
    }

    if(both_min == false)
    {
        if(t_min2 != pd[1] || first_run == true)
        {
            mins[1].erase_it();
            mins[1].show(pd[1]);
        }
    }

    t_min1 = pd[0];
    t_min2 = pd[1];
    both_min = false;
    delete [] pd;

}

void Clock::hours()
{
    if(t_hour1 != pd[0] && t_hour2 != pd[1])
    {
        hour[0].erase_it();
        hour[1].erase_it();
        hour[0].show(pd[0]);
        hour[1].show(pd[1]);
        both_hour = true;
    }

    if(both_hour == false || first_run == true)
    {
        if(t_hour2 != pd[1] || first_run == true)
        {
            hour[1].erase_it();
            hour[1].show(pd[1]);
        }
    }

    t_hour1 = pd[0];
    t_hour2 = pd[1];
    both_hour = false;
    delete [] pd;

}

void Clock::stop()
{
    first_run = true;
    t_s1 = 0;
    t_s2 = 0;
    t_min1 = 0;
    t_min2 = 0;
    t_hour1 = 0;
    t_hour2 = 0;
    both_sec = false;
    both_min = false;
    both_hour = false;
    first_run = true;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

DHTsensor::DHTsensor() : dht(d)
{
    temperature = humidity = 0.0;
    tmark.set_position(1,1);
    hmark.set_position(21,1);
    tempdis[0].set_position(9,1);
    tempdis[1].set_position(13,1);
    humdis[0].set_position(28,1);
    humdis[1].set_position(32,1);
}

void DHTsensor::begin()
{
    dht.begin();
}

void DHTsensor::read()
{
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
}

int * DHTsensor::no_separator(const int & no)
{
    int tab_size = 0;
    if(no/10 == 0)
    {
        tab_size = 1;
    }
    else
    {
        tab_size = 2;
    }

    int *help_tab = new int[tab_size];
    help_tab[0] = 0;
    help_tab[1] = 0;
    int number = no;
    int i = 0;
    int cyfra = 0;
    int z;

    while(number>0)
    {
        cyfra = number % 10;
        help_tab[i] = cyfra;
        number /= 10;
        i++;
    }

    z = help_tab[0];
    help_tab[0] = help_tab[1];
    help_tab[1] = z;
    return help_tab;

}

void DHTsensor::show_dht()
{
    read();
    tempdis[0].set_text_colors(0,50,50);
    hmark.set_text_colors(0,50,50);
    tmark.set_position(1,1);
    tmark.show("T%col");
    pd = no_separator(temperature);
    show_temp();
    tmark.set_position(15,1);
    tmark.show("%deg");
    hmark.set_position(21,1);
    hmark.show("H%col");
    pd = no_separator(humidity);
    show_hum();
    hmark.set_position(35,1);
    hmark.show("%per");
}

void DHTsensor::show_temp()
{
    if(t1 != pd[0] && t2 != pd[1])
    {
        tempdis[0].erase_it();
        tempdis[1].erase_it();
        tempdis[0].show(pd[0]);
        tempdis[1].show(pd[1]);
        both_temp = true;
    }

    if(both_temp == false)
    {
        if(t2 != pd[1])
        {
            tempdis[1].erase_it();
            tempdis[1].show(pd[1]);
        }
    }

    t1 = pd[0];
    t2 = pd[1];
    both_temp = false;
    delete [] pd;

}

void DHTsensor::show_hum()
{
    if(h1 != pd[0] && h2 != pd[1])
    {
        humdis[0].erase_it();
        humdis[1].erase_it();
        humdis[0].show(pd[0]);
        humdis[1].show(pd[1]);
        both_hum = true;
    }

    if(both_hum == false)
    {
        if(h2 != pd[1])
        {
            humdis[1].erase_it();
            humdis[1].show(pd[1]);
        }
    }

    h1 = pd[0];
    h2 = pd[1];
    both_hum = false;
    delete [] pd;
}
