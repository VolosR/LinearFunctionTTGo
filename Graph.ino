#include <TFT_eSPI.h>                
TFT_eSPI    tft = TFT_eSPI();  
#define gray  0x4228

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

int x=4;
int y=4;
int width=120;
int boxes=6;

int graph[4];
int pointsX[20];
int pointsY[20];

float cor[4]={0};
int counter=0;
int value=-3;

int debounce1=0;
int debounce2=0;

int sp[4]={160,160,182,182};
int spy[4]={44,64,44,64};

float slope;
void setup() {
  Serial.begin(9600);
  pinMode(0,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 80 );

  drawAll();
}


void loop() {

  if(digitalRead(0)==0)
  {
    if(debounce1==0)
  
    {
      debounce1=1;
      value++;
      if(value>3)
      value=-3;

      cor[counter]=value;

      drawAll();
      delay(50);
      
    }
  }else{debounce1=0;}
  
 if(digitalRead(35)==0)
  {
    if(debounce2==0)
  
    {
    debounce2=1;
    counter++;
    if(counter>3)
    counter=0;

     tft.drawString(String(counter),125,125,2);
     tft.fillRect(7,159,7+7,53,TFT_BLACK);
     tft.setTextColor(TFT_RED ,TFT_BLACK);
     tft.drawString("@",8,sp[counter],2);
     tft.setTextColor(TFT_WHITE ,TFT_BLACK);
     tft.fillRect(0,sp[0]+18,135,2,TFT_BLACK);
     tft.fillRect(0,sp[2]+18,135,2,TFT_BLACK);
     tft.fillRect(spy[counter]+7,sp[counter]+18,11,2,TFT_YELLOW);
    delay(200);
    }
  }else{debounce2=0;}

}
void drawGraph(int x, int y, int boxes, int w)
{
    tft.setTextColor(TFT_WHITE ,TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
   int n=x;
   int m=y;
   int j=0;
   for(int i=(boxes/2)*-1; i<=boxes/2; i++){
    pointsX[j]=n;
    pointsY[j]=m;
    Serial.println(pointsY[j]);
    j++;
    tft.drawLine(n,y,n,y+w,gray);
    tft.drawLine(x,m,x+w,m,gray);
    tft.drawLine(n,w/2+y-2,n,w/2+y+2,TFT_GOLD);
    tft.drawLine(w/2+x-2,m,w/2+x+2,m,TFT_GOLD);
    if(i!=0)
    if(i<0)
    {
    tft.drawString(String(i),n-8,y+(w/2)+7,1);  
    tft.drawString(String(i*-1),x+7+w/2,m-3,1);
    }
    else
    {
      tft.drawString(String(i*-1),x+7+w/2,m-3,1); 
      tft.drawString(String(i),n-2,y+(w/2)+7,1); 
    }
    n=n+w/boxes; 
    m=m+w/boxes;  
  }
    tft.drawLine(x,w/2+y,w+x,w/2+y,TFT_WHITE);
    tft.drawLine(w/2+x,y,w/2+x,w+y,TFT_WHITE);
    graph[0]=x;
    graph[1]=y;
    graph[2]=boxes;
    graph[3]=w;

   tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  }
 void drawPoint(int x, int y)
    {   
    tft.fillCircle(pointsX[boxes/2+x],pointsY[boxes/2-y],4,TFT_RED);
    }

String linear(float x1, float y1, float x2,float y2)
{
  String res="";
     float a,b,c;
    a=x2-x1;
    b=y2-y1;
    c=(x2*y1)-(x1*y2);

    String first,second;

    if((b/a)<0)
    first="-";
    else
    first="";

     if((c/a)<0)
    second="-";
    else
    second="+";

    if(x1==x2)
    res="x="+String(int(x1));
    else if(y2==y1)
    res="y="+String(int(y1));
    else if(x1==0 && x2==0 && y1==0 && y2==0)
    res="";
    else{
        if((c/a)<0)
        res="y= "+String(b/a)+" x "+second+" "+String((c/a)*-1);
        else
        res="y= "+String(b/a)+" x "+second+" "+String((c/a));
        
    }
    return res;
}

void drawF(int x, int y, int x2,int y2)
  {
   
    
    tft.drawLine(pointsX[boxes/2+x],pointsY[boxes/2-y],pointsX[boxes/2+x2],pointsY[boxes/2-y2],TFT_RED);
    slope=(float)(y2-y)/(float)(x2-x);
    
    float cx=0;
    float cy=0;


    
    if(x!=x2){
    for(int i=pointsY[boxes/2-y];i<=pointsY[boxes];i++)
      {
      cx=pointsX[boxes/2+x]+i;
      cy=pointsY[boxes/2-y]-i*slope;
      
      } 
    tft.drawLine(pointsX[boxes/2+x],pointsY[boxes/2-y],cx,cy,TFT_RED);
    
    cx=pointsX[boxes/2+x2];
    cy=pointsY[boxes/2-y2];
    for(int i=pointsX[boxes/2+x2];i>=pointsX[0];i--)
      {
        cx=cx-1;
        cy=cy+slope;
        }
     tft.drawLine(pointsX[boxes/2+x2],pointsY[boxes/2-y2],(int)cx,(int)cy,TFT_RED);    
    }else {tft.drawLine(pointsX[boxes/2+x],pointsY[0],pointsX[boxes/2+x],pointsY[6],TFT_RED);}
  }

  void drawAll()
  {
          drawGraph(8,24,6,120);
  drawPoint(cor[0],cor[1]);
  drawPoint(cor[2],cor[3]);
  drawF((int)cor[0],(int)cor[1],(int)cor[2],(int)cor[3]);

  tft.fillRect(0,148,135,240,TFT_BLACK);
  tft.setTextColor(TFT_GOLD,TFT_BLACK);
  tft.drawString("LINEAR FUNCTION",8,0,2);
  tft.setTextColor(TFT_WHITE ,0x2880);
  tft.drawString( " A( "+String((int)cor[0])+" , "+String((int)cor[1])+" ) ",22,160,2);
  tft.drawString(" B( "+String((int)cor[2])+" , "+String((int)cor[3])+" ) ",22,182,2);
  tft.fillRect(spy[counter]+7,sp[counter]+18,11,2,TFT_YELLOW);
   tft.setTextColor(TFT_RED ,TFT_BLACK);
     tft.drawString("@",8,sp[counter],2);
     
  tft.setTextColor(0x5E37,TFT_BLACK);
  tft.drawString(linear(cor[0],cor[1],cor[2],cor[3]),8,208,2);
  tft.drawString("slope= "+String(slope),8,226,2);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
    
    }
    
