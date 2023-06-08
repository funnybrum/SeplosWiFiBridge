const char CONFIG_PAGE[] PROGMEM = R"=====(
<HTML>
 <HEAD>
   <TITLE>Battery WiFi Bridge</TITLE>
 </HEAD>
 <BODY>
  <form action="/settings" method="get">
   %s<br><br>
   %s<br><br>
   <br><br>
   <input type="submit" value="Save" style='width: 150px;'>
   &nbsp;&nbsp;&nbsp;
   <a href="/reboot">
    <button type="button" style='width: 150px;'>Restart</button>
   </a>
  </form>
 </BODY>
</HTML>
)=====";
/*
 "power":%.0f,
 ...
 "cell1_v": %.3f,
 "cell2_v": %.3f,
 "cell3_v": %.3f,
 "cell4_v": %.3f,
 "cell5_v": %.3f,
 "cell6_v": %.3f,
 "cell7_v": %.3f,
 "cell8_v": %.3f,
 "cell9_v": %.3f,
 "cell10_v": %.3f,
 "cell11_v": %.3f,
 "cell12_v": %.3f,
 "cell13_v": %.3f,
 "cell14_v": %.3f,
 "cell15_v": %.3f,
 "cell16_v": %.3f,
*/
const char GET_JSON[] PROGMEM = R"=====({
 "voltage":%.2f,
 "port_voltage": %.2f,
 "current":%.2f,
 "cell_max": %.3f,
 "cell_min": %.3f,
 "soc": %.1f,
 "heating": %d,
 "comm_max_frame_delay": %d,
 "rssi":%d
})=====";
