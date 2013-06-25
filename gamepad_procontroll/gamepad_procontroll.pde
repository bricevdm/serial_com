import procontroll.*;

/* Controller test :) */

ControllIO controll;
ControllDevice device;
ControllStick stick1;
ControllStick stick2;

ControllButton button1;
ControllButton button2;
ControllButton button3;
ControllButton button4;

void setup(){
  
  size(320, 180);
  smooth();
  noStroke();
  frameRate(60);
  background(80);
  ellipseMode(CENTER);

  controll = ControllIO.getInstance(this);
  //controll.printDevices();
  device = controll.getDevice(0);
  device.printSliders();
  device.printButtons();
  device.printSticks();
  
  stick1 = device.getStick(0);
  try{
    stick2 = device.getStick(1);
  }catch(Exception e){
    stick2 = null;
  }
  
  button1 = device.getButton(0);
  button2 = device.getButton(1);
  button3 = device.getButton(2);
  button4 = device.getButton(3);
  
  
}

void draw(){
   fill(80);
  rect(0, 0, width, height);

  fill(0);
  ellipse(width/3 + stick1.getY() * 25 - 50, height/2 + stick1.getX() * 25, 25, 25);

  if(stick2 != null){
      ellipse(width/3 + stick2.getY() * 25 + 50, height/2 + stick2.getX() * 25, 25, 25);
  }

  fill(255, button1.pressed() ? 255 : 25);
  ellipse(2*width/3, height/2 - 25, 15, 15);

  fill(255, button2.pressed() ? 255 : 25); 
  ellipse(2*width/3, height/2 + 25, 15, 15);
  
  fill(255, button3.pressed() ? 255 : 25); 
  ellipse(2*width/3 + 50, height/2 - 25, 15, 15);
  
  fill(255, button4.pressed() ? 255 : 25); 
  ellipse(2*width/3 + 50, height/2 + 25, 15, 15);
  
}
