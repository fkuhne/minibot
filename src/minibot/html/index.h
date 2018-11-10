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
          border-radius: 10%;
        }
        .center {
          position: relative;
          top   : 350px;
          left  : 350px;
          width: 100px;
          height: 100px;
          border: 5px blue;
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
      <title>Minibot: controlling a robot with mobile inertial sensors (Felipe Kühne)</title>
    </head>
    <body>
      <div class="garden">
          <div class="ball"></div>
      </div>
      <pre class="output"></pre>
      <canvas id="circlecanvas" width="100" height="100"></canvas>

      <script>
        var canvas = document.getElementById("circlecanvas");
        var context = canvas.getContext("2d");
        context.arc(50, 50, 50, 0, Math.PI * 2, false);
        context.fillStyle = "red";
        context.fill()

        var connection = new WebSocket('ws://' + window.location.hostname + ':81/');
        connection.onopen = function () {
          connection.send('Connect ' + new Date());
        };
        connection.onerror = function (error) {
          console.log('WebSocket Error ', error);
        };
        connection.onmessage = function (e) {
          console.log('Server: ', e.data);
        };
        connection.onclose = function () {
          console.log('WebSocket connection closed');
        };

        var running = 0, getInitialPosition = 1;
        var initialX = 0, initialY = 0;
        var x = 0, y = 0;
        var ball   = document.querySelector('.ball');
        var garden = document.querySelector('.garden');
        var output = document.querySelector('.output');
        var maxX = garden.clientWidth  - ball.clientWidth;
        var maxY = garden.clientHeight - ball.clientHeight;

        function precise(x) {
          return Number.parseFloat(x).toFixed(0);
        }

        function run(){
          if(running == 0) {
            running = 1;
            getInitialPosition = 0;
            var cmd = "run";
            connection.send(cmd);
          }
        }

        function stop(){
          if(running == 1) {
            getInitialPosition = 1;
            running = 0;
            var cmd = "stop";
            connection.send(cmd);
          }
        }

        function handleOrientation(event){
          if(getInitialPosition == 1) {
            initialX = event.beta;
            initialY = event.gamma;
          }

          //if(running == 1) {
            x = event.beta - initialX;  // In degree in the range [-180,180]
            y = event.gamma - initialY; // In degree in the range [-90,90]
          /*} else {
            x = 0;
            y = 0;
          }*/

          output.innerHTML  = "beta : " + precise(-x) + "\n";
          output.innerHTML += "gamma: " + precise(y) + "\n";
          output.innerHTML += "run  : " + running + "\n";

          let posX = x, posY = y;

          // Because we don't want to have the device upside down
          // We constrain the x value to the range [-90,90]
          //if(running == 1) {
            if (posX >  90) { posX =  90};
            if (posX < -90) { posX = -90};
            // To make computation easier we shift the range of
            // x and y to [0,180]
            posX += 90;
            posY += 90;
            ball.style.top  = (maxX*posX/180) + "px";
            ball.style.left = (maxY*posY/180) + "px";

          /*} else {
            ball.style.top  = "350px";
            ball.style.left = "350px";
          }*/
        }

        function updateDevicePosition() {
          var data = precise(-x) + ";" + precise(y);
          connection.send(data);
          window.setTimeout(updateDevicePosition, 250);
        }
        window.addEventListener('deviceorientation', handleOrientation);
        window.setTimeout(updateDevicePosition, 250);
      </script>
      <center>
        <input type='button' style='font-size:80px' value='RUN' onclick='run()'>
        <input type='button' style='font-size:80px' value='STOP' onclick='stop()'>
      </center>
    </body>
  </html>
)=====";