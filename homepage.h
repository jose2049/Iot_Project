// To create literal strings stored in flash memory enclose your HTML code between 
// F(R"=====( HTML code here )=====");
// If you have 1 reading then you probably have 2 literal strings
// If you have 2 readings then you probably have 3 literal strings etc.

String homePagePart1 = F(R"=====(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Sensor Readings</title>

<style>
body { background-color: DodgerBlue; }
.flex-Container {
  display: flex;
  flex-direction: column;
  align-items: center;
}
h1 {
  font-size: 40px;
  font-family: Arial;
  color: navy;
  text-align: center;
}
p {
  font-size: 25px;
  font-family: Arial;
  color: navy;
  text-align: center;
}
table, th, td {
  font-size: 25px;
  padding: 8px;
  border-bottom: 1px solid #ddd;
}
</style>
</head>
<body>
  <div class="flex-Container">
    <h1>Light Pollution Sensor Website</h1>
    <p>Welcome to my website displaying the data from the sensors</p>

    <table>
      <tr>
        <th>Sensor</th>
        <th>Value</th>
        <th>Unit</th>
      </tr>
)=====");

String homePagePart2 = F(R"=====(
      </table>
    </div>
  </body>
</html>
)=====");

