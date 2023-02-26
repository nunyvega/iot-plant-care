<?php

$dbConfig = [
  'host' => 'localhost',
  'name' => 'fxuntumy_iot_project',
  'user' => 'fxuntumy_nunyvega',
  'pass' => 'fxuntumy_!!',
];

$conn = new mysqli($dbConfig['host'], $dbConfig['user'], $dbConfig['pass'], $dbConfig['name']);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT id, value1, reading_time FROM Sensor ORDER BY reading_time DESC LIMIT 40";

$result = $conn->query($sql);
$sensorData = [];
while ($data = $result->fetch_assoc()){
    $sensorData[] = $data;
}

$readingsTime = array_column($sensorData, 'reading_time');

$value1 = json_encode(array_reverse(array_column($sensorData, 'value1')), JSON_NUMERIC_CHECK);
$readingTime = json_encode(array_reverse($readingsTime), JSON_NUMERIC_CHECK);

$result->free();
$conn->close();

?>

<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="refresh" content="300">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://code.highcharts.com/highcharts.js"></script>
    <style>
      body {
        min-width: 310px;
        max-width: 1280px;
        height: 500px;
        margin: 0 auto;
      }
      h2 {
        font-family: Arial;
        font-size: 2.5rem;
        text-align: center;
      }
    </style>
  </head>
  <body>
    <h2>Alvaro's plant soil moisture levels</h2>
    <div id="chart-soilmoisture" class="container"></div>
    <br>
    <br>
    <img src="http://iot.bluehost-testsite-nunyvega.blog/uploads/camera.jpg" alt="Bonsai Tree" style="width:500px;margin: auto;display: flex;">
    <script>
      var value1 = <?php echo $value1; ?>;
      var readingTime = <?php echo $readingTime; ?>;

      var chartT = new Highcharts.Chart({
        chart: { renderTo: 'chart-soilmoisture' },
        title: { text: 'Soil Moisture Level' },
        series: [{
          showInLegend: false,
          data: value1
        }],
        plotOptions: {
          line: {
            animation: false,
            dataLabels: { enabled: true }
          },
          series: { color: '#059e8a' }
        },
        xAxis: {
          type: 'datetime',
          categories: readingTime
        },
        yAxis: {
          title: { text: 'Relative Moisture' }
        },
        credits: { enabled: false }
      });
    </script>
  </body>
</html>
