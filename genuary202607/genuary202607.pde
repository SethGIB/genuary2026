class Agent
{
  PVector Movement; 
  float Step = 1.0;
  int Weight = 0;
  int Age = 0;
  int Lifespan = 0;
  boolean isFilled;
  
  //draw params
  PVector tl;
  int transp = 0;
  
  Agent(){}
  
  Agent(PVector _d, int _w, int _a)
  {
    this.Movement = new PVector(_d.x, _d.x);
    this.Step = _d.y;
    this.Weight = _w;
    this.Age = _a;
    this.Lifespan = _a;
    
    this.reset(false);
  }
  
  void step()
  {
    --this.Age;
    if(this.Age == 0)
    {
        this.reset(true);
    }
    
    this.tl.x += this.Movement.x*this.Step;
    this.tl.y += this.Movement.y*this.Step;
    
    this.transp = int(map(this.Age, this.Lifespan, 0, 255, 0));
    
  }
  
  void showMask(PGraphics dest)
  {
    dest.noFill();
    dest.stroke(255,255,255,255);
    
    int w = int(map(this.Age, this.Lifespan, 0, this.Weight, 1));
    dest.strokeWeight(w);
    dest.ellipse(0.0,0.0,this.tl.x, this.tl.y);        
  }
  
  void show()
  {
    noFill();
    stroke(255,255,255,this.transp);
    
    int w = int(map(this.Age, this.Lifespan, 0, this.Weight, 1));
    strokeWeight(w);
    ellipse(0.0,0.0,this.tl.x, this.tl.y);    
  }
  
  void reset(boolean recalcAll)
  {
    if (recalcAll == true)
    {
      this.Movement.x = random(MOVEDIMS.x, MOVEDIMS.y);
      this.Movement.y = this.Movement.x;
      this.Step = random(STEPDIMS.x, STEPDIMS.y);
      
      this.Weight = int(random(WEIGHTDIMS.x, WEIGHTDIMS.y));
      this.Age = int(random(AGEDIMS.x, AGEDIMS.y));
      
      this.isFilled = random(1.0) < 0.5;
    }

    boolean neg = random(1.0) < 0.5;
    if(neg)
      this.Movement.x *= -1;
    neg = random(1.0) < 0.5;
    if(neg)
      this.Movement.y *= -1;      
      
    this.tl = new PVector(this.Movement.x*this.Step, this.Movement.y*this.Step);
  }
}

int WIDTH = 540;
int HEIGHT = 960;
int AGENTS = 100;
float AMP = 12;
PVector MOVEDIMS = new PVector(3,30);
PVector STEPDIMS = new PVector(0.01,0.25);
PVector WEIGHTDIMS = new PVector(1,20);
PVector AGEDIMS = new PVector(30,150);
int SPAWN_DELAY = 15;
boolean GLOBAL_ROTATION = false;
ArrayList<Agent> agents = new ArrayList();

PGraphics stencilBuffer;

boolean ISRUNNING = false;

void settings()
{
  size(WIDTH,HEIGHT);  
}

void setup()
{
  ellipseMode(CORNERS);
  background(0,0,0,255);  
  stencilBuffer = createGraphics(WIDTH, HEIGHT);
  stencilBuffer.ellipseMode(CORNERS);
}

void draw()
{
  background(0,0,0,255);

  if(ISRUNNING == true)
  {
    if(agents.size() < AGENTS && (frameCount % SPAWN_DELAY==0))
    {
      SPAWN_DELAY = (int)random(10,60);
      float mx = random(MOVEDIMS.x, MOVEDIMS.y);
      float my = random(STEPDIMS.x, STEPDIMS.y);
      int wt = int(random(WEIGHTDIMS.x, WEIGHTDIMS.y));
      int ag = int(random(AGEDIMS.x, AGEDIMS.y));
      
      agents.add(new Agent(new PVector(mx,my), wt, ag));    
    }
  
    int milli = millis();
    int frames = frameCount;
    
    if(agents.size() > 0)
    {  
      for(Agent a : agents)
      {
        a.step();
      }
      
      drawBuffer(milli, frames);
      drawBase(milli, frames);
      
      blendMode(MULTIPLY);
      image(stencilBuffer,0,0);
    }
  }
}

void drawBase(int mTime, int fCount)
{
    pushMatrix();
  
    translate(WIDTH/2, HEIGHT/2);
    if(GLOBAL_ROTATION == true)
      rotate( (10*cos(mTime*0.0001)) * sin(fCount * 0.001) );
  
    blendMode(BLEND);

    int id = 0;
    for(Agent a : agents)
    {
      if(GLOBAL_ROTATION == false) {
        float amp = AMP;
        if(id % 2 == 0)
        {
          amp = -amp;
        }
        rotate( (amp*cos(mTime*0.0001)) * sin(id-(fCount * 0.0001)) );
      }
      a.show();
      id+=1;
    }
    
    popMatrix();      
}

void drawBuffer(int mTime, int fCount)
{
    stencilBuffer.beginDraw();
    stencilBuffer.background(0,0,0,255);
    stencilBuffer.ellipseMode(CORNERS);
    stencilBuffer.pushMatrix();
    stencilBuffer.translate(WIDTH/2, HEIGHT/2);
    if(GLOBAL_ROTATION == true)
      stencilBuffer.rotate( (10*cos(mTime*0.0001)) * sin(fCount * 0.001) );
    stencilBuffer.blendMode(EXCLUSION);
    
    int sbid = 0;
    for(Agent a : agents)
    {
      if(GLOBAL_ROTATION == false) {
        float amp = AMP;
        if(sbid % 2 == 0)
        {
          amp = -amp;
        }        
        stencilBuffer.rotate( (amp*cos(mTime*0.0001)) * sin(sbid-(fCount * 0.0001)) );      
      }
      a.showMask(stencilBuffer);
      sbid+=1;
    }

    stencilBuffer.popMatrix();
    stencilBuffer.endDraw();  
}

void keyPressed()
{
  ISRUNNING = !ISRUNNING;
}
