String homePagePart1 = R"=====(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Sensor Readings</title>

<script>
async function updateSensors() {
    try {
        const temp = await (await fetch("/temperature")).text();
        const hum  = await (await fetch("/humidity")).text();
        const bpi  = await (await fetch("/bpi")).text();
        const tvoc = await (await fetch("/tvoc")).text();
        const eco2 = await (await fetch("/eco2")).text();

        document.getElementById("temp").textContent = temp;
        document.getElementById("temp").style.color = "blue";
        document.getElementById("hum").textContent = hum;
        document.getElementById("hum").style.color = "purple";

        // Convert BPI to number safely
        let bpiValue = parseFloat(bpi.trim());
        if (isNaN(bpiValue)) bpiValue = 0;

        let level = "";
        let color = "black";

        if (bpiValue > 0.65) {
            level = "High";
            color = "red";
        } 
        else if (bpiValue > 0.45) {
            level = "Moderate";
            color = "orange";
        } 
        else {
            level = "Low";
            color = "green";
        }

        document.getElementById("bpi").textContent = bpiValue.toFixed(2);
        const unitCell = document.getElementById("bpi_unit");
        unitCell.textContent = level;
        unitCell.style.color = color;

        document.getElementById("tvoc").textContent = parseFloat(tvoc).toFixed(0);
        document.getElementById("eco2").textContent = parseFloat(eco2).toFixed(0);
    }
    catch (error) {
        console.log("Error updating sensors:", error);
    }
}

// Update every second
setInterval(updateSensors, 1000);
updateSensors(); // initial call
</script>

<style>
body {
    font-family: Arial, sans-serif;
    text-align: center;
    background: linear-gradient(to right, #74ebd5, #ACB6E5);
    margin: 0;
    padding: 20px;
}

h1 {
    color: #333;
}

p {
    color: #555;
}

.flex-Container {
    background: white;
    padding: 20px;
    border-radius: 12px;
    width: 90%;
    max-width: 600px;
    margin: auto;
    box-shadow: 0 4px 15px rgba(0,0,0,0.2);
}

table {
    margin: 20px auto;
    border-collapse: collapse;
    width: 100%;
}

th {
    background-color: #4CAF50;
    color: white;
    padding: 10px;
}

td {
    padding: 10px;
    border-bottom: 1px solid #ddd;
    transition: all 0.3s ease;
}

/* Zebra striping */
tr:nth-child(even) {
    background-color: #f2f2f2;
}

/* Hover effect */
tr:hover {
    background-color: #ddd;
}

/* Sensor value styling */
td[id] {
    font-weight: bold;
    font-size: 1.1em;
}
#bpi_unit {
    font-weight: bold;
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

<tr>
  <td style="color: blue;">Temperature</td>
  <td id="temp" style="color: blue;">--</td>
  <td style="color: blue;">°C</td>
</tr>
<tr>
  <td style="color: purple;">Humidity</td>
  <td id="hum" style="color: purple;">--</td>
  <td style="color: purple;">%</td>
</tr>
<tr>
  <td style="color: deepskyblue;">Blue Light</td>
  <td id="bpi" style="color: deepskyblue;">--</td>
  <td id = "bpi_unit">Level</td>
</tr>
<tr>
  <td style="color: brown;">TVOC</td>
  <td id="tvoc" style="color: brown;">--</td>
  <td style="color: brown;">ppb</td>
</tr>
<tr>
  <td>eCO2</td>
  <td id="eco2">--</td>
  <td>ppm</td>
</tr>

)=====";
String homePagePart2 = R"=====(

</table>
</div>
</body>
</html>

)=====";