import fip.*;

int G_W = 2000;
int G_H = 2000;
int G_CELL = 10;
int G_COLS = G_W/G_CELL;
int G_ROWS = G_H/G_CELL;
float[][] G_Z_VALS;

PShader glow;
void setup()
{
  size(540,960,P3D);
  G_Z_VALS = new float[G_COLS][G_ROWS];
  glow = loadShader(FIP.bloom);
  glow.set("intensity",0.75f);
  glow.set("glow",1.1f);
}

void draw()
{
  float rOff = -millis()*0.001;
  for(int row=0;row<G_ROWS;row++)
  {
    float cOff = 0;
    for(int col=0;col<G_COLS;col++)
    {
      G_Z_VALS[col][row] = constrain(map(noise(cOff,rOff),0.0f,1.0f,-200,200),-81,200); 
      cOff+=0.05;      
    }
    rOff+=0.02;    
  }
  
  background(64, 107, 210);
  stroke(16, 24, 56);
  pushMatrix();
  translate(width/2,height/2);
  rotateX(PI*0.43);
  translate(-G_W/2,-G_H/2);
  for(int row=0;row<G_ROWS-1;row++)
  {
    beginShape(POINTS);
    strokeWeight(1);
    for(int col=0;col<G_COLS;col++)
    {
      float seed = G_Z_VALS[col][row];
      if(seed<-80)
        fill(38,104,120);
      else if(seed >-80 && seed < 0)
        fill(42,120,38);
      else if(seed > 0 && seed < 40)
        fill(94,74,34);
      else if(seed >40)
        fill(255,255,255);
      vertex(col*G_CELL,row*G_CELL, G_Z_VALS[col][row]);
      vertex(col*G_CELL,(row+1)*G_CELL, G_Z_VALS[col][row+1]);
    }
    endShape();
  }
  popMatrix();
  
  filter(glow);
}
