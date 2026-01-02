import fip.*;

class circ
{
  PVector pos;
  public circ(float _x, float _y)
  {
    this.pos = new PVector(_x, _y);
  }
}

class ring
{
  int Count;
  float Radius;
  float Radius_2; //particle radius
  PVector Speed;
  PVector Theta;
  int Transp;
  int Weight;
  PVector Center;
  circ[] Circles;
  
  public ring(int _count, float _radius, float _radius_2, float _w, float _h)
  {
    this.Circles = new circ[_count];
    this.Count = _count;
    this.Radius = _radius;
    this.Radius_2 = _radius_2;
    this.Center = new PVector(_w/2, _h/2);
    
    this.Speed = new PVector(0,0,0);
    this.Theta = new PVector(0,0,0);
    this.Speed.x = random(-0.0125f,0.0125f)+sin(millis() * 0.00000025f);
    this.Speed.y = random(-0.0125f,0.0125f)+sin(millis() * 0.00000025f);
    this.Speed.z = random(-0.0125f,0.0125f)+sin(millis() * 0.00000025f);
    
    this.Theta.x = random(-PI, PI);
    this.Theta.y = random(-PI, PI);
    this.Theta.z = random(-PI, PI);
            
    this.Transp = int(random(32,128));
    this.Weight = int(random(1,6));
    float th = TWO_PI/this.Count;
    for(int i = 0; i < this.Count; i++)
    {
      float a = th * i;
      float cx = this.Radius * cos(a);
      float cy = this.Radius * sin(a);
      
      this.Circles[i] = new circ(cx, cy);
    }
  }
  
  public void step()
  {
    this.Theta.add(this.Speed);
  }
  
  public void show(PGraphics _buffer)
  {
    _buffer.noFill();
    _buffer.blendMode(ADD);
    _buffer.stroke(255,160,64,this.Transp);
    _buffer.strokeWeight(this.Weight);
    
    _buffer.pushMatrix();
    _buffer.translate(this.Center.x, this.Center.y);

    _buffer.rotateY(this.Theta.y);
    _buffer.rotateZ(this.Theta.z);
    _buffer.rotateX(this.Theta.x);
    
    for(int j=0;j<this.Count;j++)
    {
      _buffer.circle(this.Circles[j].pos.x,this.Circles[j].pos.y,this.Radius_2);
    }
    _buffer.popMatrix();
  }
}

int G_NUM_RINGS = 30;
ring[] G_RINGS;
PGraphics G_BUFFER;
PShader G_CRT;
PShader G_BLOOM;

void setup()
{
  size(900,900,P2D);
  G_BUFFER = createGraphics(900,900,P3D);
  G_RINGS = new ring[G_NUM_RINGS];
  float r_0 = width*0.125f;
  float r_1 = width*0.45f;
  for(int r=0;r<G_NUM_RINGS;r++)
  {
    float rad = random(r_0, r_1);
    int num = int(random(20, 50));
    
    float rad2 = map(rad, r_0, r_1, 25, 1);
    G_RINGS[r] = new ring(num, rad, rad2, width, height);
  }
  background(0);
  G_CRT = loadShader(FIP.crt);
  
  G_CRT.set("scanlineWeight", 0.2);
  G_CRT.set("brightness", 2.5);
  G_CRT.set("distortion", 0.02);
  
  G_BLOOM = loadShader(FIP.bloom);
  G_BLOOM.set("intensity", 0.25f);
  G_BLOOM.set("glow", 0.91f);
}

void draw()
{
  G_BUFFER.beginDraw();
  G_BUFFER.background(0,0,0,255);
  for(int j=0;j<G_NUM_RINGS;j++)
  {
    G_RINGS[j].step();
    G_RINGS[j].show(G_BUFFER);
  }
  G_BUFFER.filter(G_CRT);
 
  G_BUFFER.endDraw();
  noStroke();
  blendMode(ADD);
  image(G_BUFFER,0,0);
 
  blendMode(BLEND);
  fill(0,0,0,80);
  rect(0,0,width,height);
 
  filter(G_BLOOM);

}
