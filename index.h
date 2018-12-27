const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>httpRC</title>
<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
<style>.button{
  -webkit-appearance: none; 
  -webkit-border-radius:0px; 
  -webkit-user-select: none;  /* Chrome all / Safari all */
  -moz-user-select: none;     /* Firefox all */
  -ms-user-select: none;      /* IE 10+ */
  user-select: none;
 width:200px; 
 height:1.7em; 
 font-size:2em;
}></style>
</head>

<body>
<font color="#000000"><body bgcolor="#d0d0f0">
<div style="position:fixed; left; top: 5%; padding:1%">
<button id="forwardButton" ontouchstart="OnButtonDown ('forwardButton', 'CH2=1')" ontouchend="OnButtonUp ('forwardButton', 'CH2=0')" 
        onmousedown="OnButtonDown ('forwardButton', 'CH2=1')" onmouseup="OnButtonUp ('forwardButton', 'CH2=0')" class="button">forward</button></div>

<div  style="position:fixed; right:0%; top:5%; padding:1%">
<button id="leftButton" ontouchstart="OnButtonDown ('leftButton', 'CH1=-1')" ontouchend="OnButtonUp ('leftButton', 'CH1=0')"         
        onmousedown="OnButtonDown ('leftButton', 'CH1=-1')" onmouseup="OnButtonUp ('leftButton', 'CH1=0')" class="button">left</button></div>


<div  style="position:fixed; left; top: 32%; padding:1%">
<button id="backwardButton" ontouchstart="OnButtonDown ('backwardButton', 'CH2=-1')" ontouchend="OnButtonUp ('backwardButton', 'CH2=0')" 
        onmousedown="OnButtonDown ('backwardButton', 'CH2=-1')" onmouseup="OnButtonUp ('backwardButton', 'CH2=0')"class="button">backward</button></div>

<div  style="position:fixed; right:0%; top:32%; padding:1%">
<button id="rightButton" ontouchstart="OnButtonDown ('rightButton', 'CH1=1')" ontouchend="OnButtonUp ('rightButton', 'CH1=0')" 
        onmousedown="OnButtonDown ('rightButton', 'CH1=1')" onmouseup="OnButtonUp ('rightButton', 'CH1=0')" class="button">right</button></div>

<script>
function OnButtonDown (ButtonID, CmdString) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "CMD?" + CmdString, true);
  xhttp.send();
  document.getElementById(ButtonID).style.color = "#ff0000";
}

function OnButtonUp (ButtonID, CmdString) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
    }
  };
  xhttp.open("GET", "CMD?" + CmdString, true);
  xhttp.send();
  document.getElementById(ButtonID).style.color = "#000000";
}

</script>
</body>
</html>
)=====";



