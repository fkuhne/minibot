const char MAIN_page[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
    <head>
      <meta charset="utf-8">
      <link href="https://developer.mozilla.org/static/build/styles/samples.37902ba3b7fe.css" rel="stylesheet" type="text/css" />
      <style type="text/css">
        .garden {
          position: relative;
          width: 800px;
          height: 800px;
          border: 20px solid red;
          border-radius: 100%;
        }
        .ball {
          position: relative;
          top   : 350px;
          left  : 350px;
          width : 100px;
          height: 100px;
          background: green;
          border-radius: 100%;
        }
        .output {
          font-size: 50px;
        }
      </style>
      <title>Detecting device orientation - Orientation_example - code sample</title>
    </head>
    <body>
      <div class="garden">
        <div class="ball"></div>
      </div>
      <pre class="output"></pre>
      <script>
        var running = 0;
        var x = 0, y = 0;
        var ball   = document.querySelector('.ball');
        var garden = document.querySelector('.garden');
        var output = document.querySelector('.output');
        var maxX = garden.clientWidth  - ball.clientWidth;
        var maxY = garden.clientHeight - ball.clientHeight;

        function run(){
          running = 1;
          window.location.assign('/run')
        }
        function stop(){
          running = 0;
          window.location.assign('/stop')
        }

        function handleOrientation(event){
          //if(running == 1){
          //  x = event.beta;  // In degree in the range [-180,180]
          //  y = event.gamma; // In degree in the range [-90,90]
          //}
          //else {
          //  x = 0;
          //  y = 0;
          //}
          x = event.beta;  // In degree in the range [-180,180]
          y = event.gamma; // In degree in the range [-90,90]

          output.innerHTML  = "beta : " + x + "\n";
          output.innerHTML += "gamma: " + y + "\n";
          output.innerHTML += "run  : " + running + "\n";

          // Because we don't want to have the device upside down
          // We constrain the x value to the range [-90,90]
          if (x >  90) { x =  90};
          if (x < -90) { x = -90};
          // To make computation easier we shift the range of
          // x and y to [0,180]
          x += 90;
          y += 90;

          // 50 is half the size of the ball
          // It center the positioning point to the center of the ball
          ball.style.top  = (maxX*x/180) + "px";
          ball.style.left = (maxY*y/180) + "px";
        }

        function updateDevicePosition() {
          window.location.assign('/orientation?x=' + x + '?y=' + y)
        }
        window.addEventListener('deviceorientation', handleOrientation);
        window.addEventListener('click', function(){console.log("running = " + running)});
        window.setTimeout(updateDevicePosition, 1000);
      </script>
      <center>
      <input type='button' style='font-size:80px' value='RUN' onclick='run()'>
      <input type='button' style='font-size:80px' value='STOP' onclick='stop()'></div>
      </center>
    </body>
  </html>
)=====";
