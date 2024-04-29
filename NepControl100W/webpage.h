//---------------------------------------------------------------------------------------------
// here we define the HTML startpage with CSS and JS, all will be saved only in memory of ESP32
//---------------------------------------------------------------------------------------------
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>NeptunePAControl-ESP32</title>
<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate" />
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="0" />
<meta name="viewport" content="width=device-width, initial-scale=1">
<!-- <link href='https://fonts.googleapis.com/css?family=Orbitron' rel='stylesheet' type='text/css'> -->
<style>
@font-face {
  font-display: swap;
  font-family: 'Orbitron';
  font-style: normal;
  font-weight: 400;
  src: url('http://dmr02.bzsax.de/fonts/orbitron-v31-latin-regular.woff2') format('woff2');
}

@font-face {
  font-display: swap;
  font-family: 'DSDIGI';
  font-style: normal;
  font-weight: 400;
  src: url('http://dmr02.bzsax.de/fonts/DSDIGI.woff') format('woff');
}

@font-face {
  font-display: swap;
  font-family: 'Digital-7 Mono';
  font-style: normal;
  font-weight: 400;
  src: url('http://dmr02.bzsax.de/fonts/digital-7-mono.woff2') format('woff2');
}

@font-face {
  font-display: swap;
  font-family: 'LCD7';
  font-style: normal;
  font-weight: 400;
  src: url('http://dmr02.bzsax.de/fonts/lcd-7.woff2') format('woff2');
}

html {
  font-family: Arial, Helvetica, sans-serif;
  display: inline-block;
  text-align: center;
}
h1 {
  font-family: 'Orbitron', sans-serif;
  font-size: 1.4rem;
  color: white;
}
hr.blue {
  border: 1px solid #1282A2;
}
.topnav {
  overflow: hidden;
  background-color: #0A1128;
}
body {
  margin: 0;
}

table,td,th {
/* empty-cells: show; */
border: 10px solid white; */
width: 100%;
border-spacing: 10px;
border-collapse: collapse;
/* border-collapse: separate;
}

td.center {
text-align: center;
font-family: 'Orbitron', Arial, Helvetica, sans-serif;
font-size: 1.8rem;
color:  #1E8449;
}

td.center_small {
text-align: center;
vertical-align:center;
/* font-family: 'Orbitron', Arial, Helvetica, sans-serif; */
font-size: 1.2rem;
color:  #1E8449;
/* width: 33%; */
}

th.center {
text-align: center;
vertical-align:center;
font-size: 1.2rem;
font-weight: bold;
color: #034078;
}

.content {
  padding: 50px;
}
.card-grid {
  max-width: 500px;
  margin: 0 auto;
  display: grid;
  grid-gap: 2rem;
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
}
.card {
  background-color: white;
  padding-bottom: 10px;
  box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
}
.card-title {
  font-size: 1.2rem;
  font-weight: bold;
  color: #034078;
}
.reading {
  font-size: 0.9rem;
  color: #1282A2;
}

.reading-digits {
  font-family: 'DSDIGI', Arial, Helvetica, sans-serif;
  font-weight: bold;
  font-size: 1.8rem;
  color: #1282A2;
}

.reading-fwd {
  font-family: 'Orbitron', Arial, Helvetica, sans-serif;
  font-size: 1.8rem;
  color:  #1E8449;
}
.reading-ref {
  font-size: 1.8rem;
  color:  #E74C3C;
}
.reading-time {
  /* font-family: 'Orbitron', sans-serif; */
  font-family: 'DSDIGI', Arial, Helvetica, sans-serif;
  font-size: 1rem;
  color:  #1282A2;
}
.tab {
    display: inline-block;
    margin-left: 3em;
}

.txLED {
  height: 60px;
  width: 60px;
  background-color: grey;
  border-radius: 50%;
  text-align: center;
  justify-content: center;
  align-items: center;
  display: flex;
}

.bandLED {
  height: 60px;
  width: 60px;
  background-color: grey;
  border-radius: 50%;
  text-align: center;
  justify-content: center;
  align-items: center;
  display: flex;
}

/* if Firefox */
@supports (-moz-appearance:none) {
.voltBOX {
  height: 70px;
  width: 130px;
  border-radius: 15px;
  background-color: grey;
  text-align: center;
  justify-content: center;
  justify-items: center;
  align-items: center;
  align-content : center;
  display: flex;
  flex-direction: column;
 }
}

/* if Chrome etc. */
@supports (-webkit-font-smoothing: antialiased) {
.voltBOX {
  height: 70px;
  width: 130px;
  border-radius: 15px;
  background-color: grey;
  text-align: center;
  justify-content: center;
  justify-items: center;
  align-items: center;
  align-content : center;
  display: block;
  flex-direction: column;
 }
}

.freqBOX {
  height: 30px;
  width: 200px;
  border-radius: 5px;
  background-color: grey;
  text-align: center;
  justify-content: center;
  align-items: center;
  display: flex;
  font-family: 'LCD7', Arial, Helvetica, sans-serif;
  font-size: 1rem;
}

.clockBOX {
  height: 40px;
  width: 280px;
  border-radius: 5px;
  background-color: grey;
  text-align: center;
  justify-content: center;
  align-items: center;
  display: flex;
  font-family: 'Digital-7 Mono', Arial, Helvetica, sans-serif;
  font-size: 1.5rem;
  /* border-bottom: 10px solid white; */
}

</style>
</head>
<body>
        <div class="topnav">
            <h1>Neptune&nbsp;PA&nbsp;Controller</h1>
        </div>
        <div class="content">
            <div class="card-grid">
                <div class="card">
                    <p class="reading">WiFi&nbsp;SSID:&nbsp;<span id="ssid"></span><br>WiFi&nbsp;RSSI:&nbsp;<span id="rssi"></span>&nbsp;dbm&nbsp;&wedgeq;&nbsp;<span id="rst"></span></p>
                    <hr class="blue">
                    <center>
                      <table>
                        <tr>
                          <th class="center">RX/TX Relais</th>
                          <th class="center">LPF/Band</th>
                          <th class="center">HL2&vrtri;BV</th>
                        </tr>
                        <tr>
                          <td class="center_small"><div class="voltBOX"><span id="rxtxrelais"></span></div></td>
                          <td class="center_small"><div class="voltBOX"><span id="band"></span></div></td>
                          <td class="center_small"><div class="voltBOX"><span id="bv"></span></div></td>
                        </tr>
                        <tr>
                          <td class="center_small" colspan="3"><center><div class="freqBOX"><span id="bias"></span></div></center></td>
                        </tr>
                        <tr>
                          <td class="center_small" colspan="3"><center><div class="clockBOX"><span id="time"></span></div></center></td>
                        </tr>
                      </table>
                    </center>
                    <!-- <hr class="blue"> -->
                </div>
            </div>
        </div>
<script>
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
// Init web socket when the page loads
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function getReadings() {
    websocket.send("getReadings");
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

// When websocket is established, call the getReadings() function
function onOpen(event) {
    console.log('Connection opened');
    getReadings();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
  console.log(event.data);
  var myObj = JSON.parse(event.data);
  console.log(myObj);

    // get trivial data
    document.getElementById('rssi').innerHTML = myObj['rssi'];
    document.getElementById('rst').innerHTML = myObj['rst'];
    document.getElementById('ssid').innerHTML = myObj['ssid'];
 
    // selected band
    document.getElementById('band').innerHTML = myObj['band'];
    document.getElementById('band').innerHTML += "m";
    document.getElementById('band').style.color = "white";

    // get BIAS
    if (myObj['bias']) {
      document.getElementById('bias').innerHTML = "MOSFET BIAS ON";
      document.getElementById('bias').style.color = "black";
      document.getElementsByClassName('freqBOX')[0].style.backgroundColor = "#ffbf00";
    } else {
      document.getElementById('bias').innerHTML = "MOSFET BIAS OFF";
      document.getElementById('bias').style.color = "black";
      document.getElementsByClassName('freqBOX')[0].style.backgroundColor = "#ffbf00";
    }

    // bandvoltage HL2 
    document.getElementById('bv').innerHTML = myObj['bv'];
    document.getElementById('bv').innerHTML += " mV";
    document.getElementById('bv').style.color = "white";

    // get RX/TX relais
    if (myObj['rxtxrelais']) {
      document.getElementById('rxtxrelais').innerHTML = "TX";
      document.getElementById('rxtxrelais').style.color = "white";
    } else {
      document.getElementById('rxtxrelais').innerHTML = "BYPASS";
      document.getElementById('rxtxrelais').style.color = "white";
    }

    // sent time last data
    document.getElementById('time').innerHTML = myObj['time'];
    document.getElementById('time').style.color = "black";
    document.getElementsByClassName('clockBOX')[0].style.backgroundColor = "#cdcdcd";

    // PTT state of HL2
    if (myObj['ptt']) {
      document.getElementsByClassName('voltBOX')[0].style.backgroundColor = "red";
      document.getElementsByClassName('voltBOX')[1].style.backgroundColor = "red";
      document.getElementsByClassName('voltBOX')[2].style.backgroundColor = "red";
      document.getElementsByClassName('topnav')[0].style.backgroundColor = "red";
    } else {
      document.getElementsByClassName('voltBOX')[0].style.backgroundColor = "green";
      document.getElementsByClassName('voltBOX')[1].style.backgroundColor = "green";
      document.getElementsByClassName('voltBOX')[2].style.backgroundColor = "green";
      document.getElementsByClassName('topnav')[0].style.backgroundColor = "#0A1128";
    }

} 
</script>
</body>
</html>
)rawliteral";
//-----------------------------------------------------------------------------------------