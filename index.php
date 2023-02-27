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

$sql = "SELECT id, moisture, humidity, airTemperature, soilTemperature, reading_time FROM Sensor ORDER BY reading_time DESC LIMIT 210";

$result = $conn->query($sql);
$sensorData = [];
while ($data = $result->fetch_assoc()) {
  $sensorData[] = $data;
}

$moisture = json_encode(array_reverse(array_column($sensorData, 'moisture')), JSON_NUMERIC_CHECK);
$humidity = json_encode(array_reverse(array_column($sensorData, 'humidity')), JSON_NUMERIC_CHECK);
$airTemperature = json_encode(array_reverse(array_column($sensorData, 'airTemperature')), JSON_NUMERIC_CHECK);
$soilTemperature = json_encode(array_reverse(array_column($sensorData, 'soilTemperature')), JSON_NUMERIC_CHECK);

$readingTime = json_encode(array_reverse(array_column($sensorData, 'reading_time')), JSON_NUMERIC_CHECK);

$result->free();
$conn->close();

// Get the latest values of each sensor
$latestMoisture = $sensorData[0]['moisture'];
$latestHumidity = $sensorData[0]['humidity'];
$latestAirTemperature = $sensorData[0]['airTemperature'];
$latestSoilTemperature = $sensorData[0]['soilTemperature'];


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

    h2,
    h1 {
      font-family: Arial;
      font-size: 2rem;
      text-align: center;
    }

    h1 {
      font-size: 3rem;
    }

    .all-charts-container {
      display: flex;
      flex-direction: column;
    }

    .chart-container {
      display: flex;
      flex: 1;
      border-radius: 30px;
      margin: 15px;
    }

    .sensor-cards {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
    }

    .sensor-card {
      flex-basis: calc(40% - 40px);
      margin: 20px;
      padding: 20px;
      text-align: center;
      background-color: #f2f2f2;
      box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2);
      border-radius: 30px;
    }

    .sensor-card h3 {
      font-size: 1.5rem;
      margin-bottom: 10px;
    }

    .sensor-card p {
      font-size: 2rem;
      font-weight: bold;
    }

    body {
      background-color: lightblue;
    }

    #img-container img {
      border-radius: 30px;
      border-color: white;
      border-style: solid;
      border-width: 15px;
    }
  </style>
</head>

<body>
  <h1>Plant Care and Monitoring System</h1>
  <h2> At a Glance</h2>
  <div class="sensor-cards">
    <div class="sensor-card">
      <h3>Moisture</h3>
      <p>
        <?php echo $latestMoisture; ?>%
      </p>
    </div>
    <div class="sensor-card">
      <h3>Humidity</h3>
      <p>
        <?php echo $latestHumidity; ?>%
      </p>
    </div>
    <div class="sensor-card">
      <h3>Air Temperature</h3>
      <p>
        <?php echo $latestAirTemperature; ?>°
      </p>
    </div>
    <div class="sensor-card">
      <h3>Soil Temperature</h3>
      <p>
        <?php echo $latestSoilTemperature; ?>°
      </p>
    </div>
  </div>

  <h2>Your Plant Now:</h2>
  <div id="img-container">
    <img src="http://iot.bluehost-testsite-nunyvega.blog/uploads/camera.jpg" alt="Bonsai Tree"
      style="width:70%;margin: auto;display: flex;">
  </div>
  <div id="all-charts-container">
    <div id="chart-moisture" class="chart-container"></div>
    <div id="chart-humidity" class="chart-container"></div>
    <div id="chart-temperature" class="chart-container"></div>
  </div>
  <br>
  <br>

  <script>
    var moisture = <?php echo $moisture; ?>;

    var humidity = <?php echo $humidity; ?>;


    var airTemperature = <?php echo $airTemperature; ?>;
    var soilTemperature = <?php echo $soilTemperature; ?>;
    var readingTime = <?php echo $readingTime; ?>;

    const formattedDates = readingTime.map(date => {
      const parsedDate = new Date(date);
      return parsedDate.toLocaleTimeString('en-US', { month: "short", day: "numeric", hour: '2-digit', minute: '2-digit', hour12: false });
    });


    var chartMoisture = new Highcharts.Chart({
      chart: { renderTo: 'chart-moisture' },
      title: { text: 'Moisture Data' },
      series: [
        { name: 'Moisture', data: moisture }
      ],
      plotOptions: {
        line: {
          animation: false,
          dataLabels: { enabled: false }
        },
        series: { color: 'DeepSkyBlue' }
      },
      xAxis: {
        type: 'datetime',
        categories: formattedDates
      },
      yAxis: {
        title: { text: 'Value' }
      },
      credits: { enabled: false }
    });

    var chartHumidity = new Highcharts.Chart({
      chart: { renderTo: 'chart-humidity' },
      title: { text: 'Humidity Data' },
      series: [
        { name: 'Humidity', data: humidity, color: 'SteelBlue' }
      ],
      plotOptions: {
        line: {
          animation: false,
          dataLabels: { enabled: true }
        }
      },
      xAxis: {
        type: 'datetime',
        categories: formattedDates
      },
      yAxis: {
        title: { text: 'Value' }
      },
      credits: { enabled: false }
    });

    var chartTemperature = new Highcharts.Chart({
      chart: { renderTo: 'chart-temperature' },
      title: { text: 'Temperature Data' },
      series: [
        { name: 'Air Temperature', data: airTemperature, color: 'DeepSkyBlue' },
        { name: 'Soil Temperature', data: soilTemperature, color: 'Sienna' },
      ],
      plotOptions: {
        line: {
          animation: false,
          dataLabels: { enabled: true }
        },
        series: { color: '#059e8a' }
      },
      xAxis: {
        type: 'datetime',
        categories: formattedDates
      },
      yAxis: {
        title: { text: 'Value' }
      },
      credits: { enabled: false }
    });
  </script>
</body>

</html>