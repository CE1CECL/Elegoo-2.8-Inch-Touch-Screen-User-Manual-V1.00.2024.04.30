// UNO R4 MONIMA Board pin usage:
// TFT-LCD Data Bit :               7    6    5    4    3    2    1    0
// UNO R4 MONIMA Board digi pin :   7    6    5    4    3    2    9    8
// UNO R4 MONIMA Board port pin : P107 P106 P102 P103 P104 P105 P303 P304
#define LCD_WR   A1
#define LCD_RS   A2
#define LCD_CS   A3
#define LCD_REST A4

static const uint16_t _xset_mask[] = {
    // 0x0     0x1     0x2     0x3     0x4     0x5     0x6     0x7
    0x0000, 0x0020, 0x0010, 0x0030, 0x0008, 0x0028, 0x0018, 0x0038,
    0x0004, 0x0024, 0x0014, 0x0034, 0x000C, 0x002C, 0x001C, 0x003C,
    0x0040, 0x0060, 0x0050, 0x0070, 0x0048, 0x0068, 0x0058, 0x0078,
    0x0044, 0x0064, 0x0054, 0x0074, 0x004C, 0x006C, 0x005C, 0x007C,
    0x0080, 0x00A0, 0x0090, 0x00B0, 0x0088, 0x00A8, 0x0098, 0x00B8,
    0x0084, 0x00A4, 0x0094, 0x00B4, 0x008C, 0x00AC, 0x009C, 0x00BC,
    0x00C0, 0x00E0, 0x00D0, 0x00F0, 0x00C8, 0x00E8, 0x00D8, 0x00F8,
    0x00C4, 0x00E4, 0x00D4, 0x00F4, 0x00CC, 0x00EC, 0x00DC, 0x00FC};

void Lcd_Writ_Bus(unsigned char d)
{
  R_PORT3->PORR = 0x18;
  R_PORT3->POSR = (d & 0x01) << 4;
  R_PORT3->POSR = (d & 0x02) << 2;
  R_PORT1->PORR = 0x00FC;
  R_PORT1->POSR = _xset_mask[d >> 2];
  digitalWrite(LCD_WR, LOW);
  digitalWrite(LCD_WR, HIGH);
}


void Lcd_Write_Com(unsigned char VH)
{
  digitalWrite(LCD_RS, LOW); //LCD_RS=0;
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Data(unsigned char VH)
{
  digitalWrite(LCD_RS, HIGH); //LCD_RS=1;
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Com_Data(unsigned char com, unsigned char dat)
{
  Lcd_Write_Com(com);
  Lcd_Write_Data(dat);
}

void Address_set(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
  Lcd_Write_Com(0x2a);
  Lcd_Write_Data(x1 >> 8);
  Lcd_Write_Data(x1);
  Lcd_Write_Data(x2 >> 8);
  Lcd_Write_Data(x2);
  Lcd_Write_Com(0x2b);
  Lcd_Write_Data(y1 >> 8);
  Lcd_Write_Data(y1);
  Lcd_Write_Data(y2 >> 8);
  Lcd_Write_Data(y2);
  Lcd_Write_Com(0x2c);
}

void Lcd_Init(void)
{
  digitalWrite(LCD_REST, HIGH);
  delay(5);
  digitalWrite(LCD_REST, LOW);
  delay(15);
  digitalWrite(LCD_REST, HIGH);
  delay(15);

  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_CS, LOW); //CS

  Lcd_Write_Com(0xCB);
  Lcd_Write_Data(0x39);
  Lcd_Write_Data(0x2C);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x34);
  Lcd_Write_Data(0x02);

  Lcd_Write_Com(0xCF);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0XC1);
  Lcd_Write_Data(0X30);

  Lcd_Write_Com(0xE8);
  Lcd_Write_Data(0x85);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x78);

  Lcd_Write_Com(0xEA);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x00);

  Lcd_Write_Com(0xED);
  Lcd_Write_Data(0x64);
  Lcd_Write_Data(0x03);
  Lcd_Write_Data(0X12);
  Lcd_Write_Data(0X81);

  Lcd_Write_Com(0xF7);
  Lcd_Write_Data(0x20);

  Lcd_Write_Com(0xC0);    //Power control
  Lcd_Write_Data(0x23);   //VRH[5:0]

  Lcd_Write_Com(0xC1);    //Power control
  Lcd_Write_Data(0x10);   //SAP[2:0];BT[3:0]

  Lcd_Write_Com(0xC5);    //VCM control
  Lcd_Write_Data(0x3e);   //Contrast
  Lcd_Write_Data(0x28);

  Lcd_Write_Com(0xC7);    //VCM control2
  Lcd_Write_Data(0x86);   //--

  Lcd_Write_Com(0x36);    // Memory Access Control
  Lcd_Write_Data(0x48);

  Lcd_Write_Com(0x3A);
  Lcd_Write_Data(0x55);

  Lcd_Write_Com(0xB1);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x18);

  Lcd_Write_Com(0xB6);    // Display Function Control
  Lcd_Write_Data(0x08);
  Lcd_Write_Data(0x82);
  Lcd_Write_Data(0x27);

  Lcd_Write_Com(0x11);    //Exit Sleep
  delay(120);

  Lcd_Write_Com(0x29);    //Display on
  Lcd_Write_Com(0x2c);
}

void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i, j;
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  Address_set(x, y, l, y);
  j = l * 2;
  for (i = 1; i <= j; i++)
  {
    Lcd_Write_Data(c);
  }
  digitalWrite(LCD_CS, HIGH);
}

void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i, j;
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  Address_set(x, y, x, l);
  j = l * 2;
  for (i = 1; i <= j; i++)
  {
    Lcd_Write_Data(c);
  }
  digitalWrite(LCD_CS, HIGH);
}

void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
}

void Rectf(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c)
{
  unsigned int i;
  for (i = 0; i < h; i++)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y + i, w, c);
  }
}
int RGB(int r, int g, int b)
{ return r << 16 | g << 8 | b;
}
void LCD_Clear(unsigned int j)
{
  unsigned int i, m;
  Address_set(0, 0, 240, 320);
  //Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  for (i = 0; i < 240; i++)
    for (m = 0; m < 320; m++)
    {
      Lcd_Write_Data(j >> 8);
      Lcd_Write_Data(j);

    }
  digitalWrite(LCD_CS, HIGH);
}

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println("2.8 inch screen.");
  for (int p = 0; p < 10; p++)
  {
    pinMode(p, OUTPUT);
  }
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A4, HIGH);

  Lcd_Init();
}

void loop()
{
  LCD_Clear(0xf800);
  LCD_Clear(0x07E0);
  LCD_Clear(0x001F);
}
