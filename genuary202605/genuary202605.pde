class gp
{
  PVector xy;
  PVector dxy;
  gp(int x, int y, int dx, int dy)
  {
    xy = new PVector(x,y);
    dxy = new PVector(dx,dy);
  }
  
  gp(PVector p, PVector d)
  {
    xy = new PVector(p.x,p.y);
    dxy = new PVector(d.x,d.y);
  }  
}

boolean G_IS_RUNNING = false;
boolean exploding;
float burstRadius = 0.25;
int NUM_PTS = 3000;
int x[] = new int[NUM_PTS];
int y[] = new int[NUM_PTS];
int dx[] = new int[NUM_PTS];
int dy[] = new int[NUM_PTS];
float shockwave;
float seismicRadius;
float epiX,epiY;
float weight,redTerm;
float screenDist,activeDist;
PImage img_mask;
PImage img_bg;
PGraphics buffer;
color iBlue;
color iYellow;
ArrayList<PVector> spawnPoints = new ArrayList();
ArrayList<gp> particles = new ArrayList();

void setup(){
  img_bg = loadImage("TX_g202605_BG.png");
  img_mask=loadImage("TX_g202605_TXT.png");
  img_mask.loadPixels();
  setupSpawners(img_mask.pixels);

  iBlue = color(161,247,201,128);
  iYellow = color(255,255,255);
  size(540,960,JAVA2D);
  ellipseMode(RADIUS);
  noStroke();
  screenDist=dist(0,0,width/2,height/2);
  buffer=createGraphics(width,height,JAVA2D);
  buffer.beginDraw();
  buffer.background(0);
  buffer.endDraw();
}
void draw(){
  blendMode(BLEND);
  image(img_bg,0,0);
  if(G_IS_RUNNING)
  {
    img_mask.loadPixels();
    stepParticles();
    drawParticles(img_mask.pixels);
    buffer.beginDraw();
    buffer.noStroke();
  
    buffer.pushStyle();
    buffer.blendMode(MULTIPLY);
    buffer.fill(0,6);
    buffer.rect(0,0,width,height);
    buffer.popStyle();
    buffer.pushStyle(); 
    buffer.blendMode(BLEND);
    for(gp g : particles)
    {
      buffer.stroke(lerpColor(iBlue,iYellow,redTerm));
      buffer.strokeWeight(lerp(1,8,redTerm));
      buffer.line(g.xy.x-g.dxy.x,g.xy.y-g.dxy.y,g.xy.x,g.xy.y);
    }
    buffer.popStyle();  
    buffer.endDraw();  
    blendMode(ADD);
    image(buffer,0,0);
    if(exploding)
    {
      if(shockwave>=seismicRadius)
      {
        exploding=false;
      }
      else
      {
        shockwave+=36;
      }
    }
  }
}

void mousePressed(){
  if(!exploding){
    epiX=mouseX;
    epiY=mouseY;
    exploding=true;
    shockwave=0;
    seismicRadius=max(abs(height-epiY),epiY);
  }
}

void setupSpawners(color[] pxlArray)
{
  for(int sy=0;sy<960;sy++)
  {
    for(int sx=0;sx<540;sx++)
    {
      int idx = sy*540+sx;
      if(pxlArray[idx] == color(255,0,0))
      {
        spawnPoints.add(new PVector(sx,sy));
      }
    }
  }
}

void keyPressed()
{
  G_IS_RUNNING = true;
}

void stepParticles()
{
  if(particles.size()<NUM_PTS)
  {
    int id=(int)random(0,spawnPoints.size());
    PVector pos = spawnPoints.get(id);
    PVector vel = new PVector(random(-1,1), random(-1,1));
    particles.add(new gp(pos,vel));
  }  
}

void drawParticles(color[] pxlArray)
{
  redTerm = 0; //for now
  for(int i=0;i<particles.size();i++)
  {
    var p = particles.get(i);
    
    //update position with velocity
    p.xy.x=p.xy.x+p.dxy.x>=width?width-1:p.xy.x+p.dxy.x;
    p.xy.y=p.xy.y+p.dxy.y>=height?height-1:p.xy.y+p.dxy.y;
    
    //generate lookup indices
    int x0=int(p.xy.x-1<=0?0:p.xy.x-1);
    int x1=int(p.xy.x+1>=width?width-1:p.xy.x+1);
    int y0=int(p.xy.y-1<=0?0:p.xy.y-1);
    int y1=int(p.xy.y+1>=height?height-1:p.xy.y+1);
    int idx0=int(p.xy.y)*width+x0;
    int idx1=int(p.xy.y)*width+x1;
    int idy0=y0*width+int(p.xy.x);
    int idy1=y1*width+int(p.xy.x);
    if(p.xy.x<=0||p.xy.x>=width||pxlArray[idx0]==color(0)||pxlArray[idx1]==color(0))
    {
      p.dxy.x*=-1;
      if(p.dxy.x<0)
      {
        //p.dxy.x = random(-1,-2);
      }
      else
      {
        //p.dxy.x = random(1,2);
      }
    }
    if(p.xy.y<=0||p.xy.y>=height||pxlArray[idy0]==color(0)||pxlArray[idy1]==color(0))
    {
      p.dxy.y*=-1;
      if(p.dxy.y<0)
      {
        //p.dxy.y = random(-1,-2);
      }
      else
      {
        //p.dxy.y = random(1,2);
      }      
    }    
  }
  /*for(int i=0;i<NUM_PTS;i++){
    //update position with velocity
    x[i]=x[i]+dx[i]>=width?width-1:x[i]+dx[i];
    y[i]=y[i]+dy[i]>=height?height-1:y[i]+dy[i];
    
    //generate texture lookup indices:
    // on on each side of the current pixel
    // one above and below the current pixel
    int x0=x[i]-1<=0?0:x[i]-1;
    int x1=x[i]+1>=width?width-1:x[i]+1;
    int y0=y[i]-1<=0?0:y[i]-1;
    int y1=y[i]+1>=height?height-1:y[i]+1;
    int idx0=y[i]*width+x0;
    int idx1=y[i]*width+x1;
    int idy0=y0*width+x[i];
    int idy1=y1*width+x[i];
    if(x[i]<=0||x[i]>=width||img_mask.pixels[idx0]==color(0)||img_mask.pixels[idx1]==color(0)){dx[i]*=-1;}
    if(y[i]<=0||y[i]>=height||img_mask.pixels[idy0]==color(0)||img_mask.pixels[idy1]==color(0)){dy[i]*=-1;}
    
    activeDist = dist(x[i],y[i],mouseX,mouseY);
    if(exploding){
      activeDist = dist(x[i],y[i],epiX,epiY);
      if(activeDist<=shockwave){
         redTerm = activeDist/shockwave;
      }else{
         redTerm = 0;
      }
    }else{
      redTerm = max(0,1-(activeDist/(screenDist*burstRadius)));
      if(mouseX>=width-10||mouseX<=10||mouseY>=height-10||mouseY<=10){
         redTerm = 0;
      }
  }*/
}
