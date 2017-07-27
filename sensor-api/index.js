const fs = require('fs')
const express = require('express')
const app = express()

const bodyParser = require('body-parser')

var { post, get } = require('./api.v0');

// parse application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: false }));

// parse application/json
app.use(bodyParser.json())
app.use(bodyParser.text({ "type": "text/*", limit: "4mb" }));

var timeout = require('connect-timeout')
app.use(timeout('5m'))

app.use(function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});

app.get('/', (req, res) => {
  res.send('Hello World!')
});

const fields = [
  "id", "lat","lng","date","time","altitude","course","speed","humidity",
  "temperature","pm1","pm25","pm10"];

const fieldTypes = {
  id: (i) => i,
  lat: (i) => parseFloat(i),
  lng: (i) => parseFloat(i),
  date: (i) => new Date(i),
  time: (i) => i,
  altitude: (i) => parseFloat(i),
  course: (i) => parseFloat(i),
  speed: (i) => parseFloat(i),
  humidity: (i) => parseFloat(i),
  temperature: (i) => parseFloat(i),
  pm1: (i) => parseInt(i),
  pm25: (i) => parseInt(i),
  pm10: (i) => parseInt(i)
};

/*
{
  "location": { x: 0.00, y: 0.00 },
  "data": {
    "pm25": 0.00,
    "pm10": 0,00
  }
}
*/
app.post('api/v0/air/:sensor_id', (req, res) => {
});

/*
  CSV ROW Description
  sensor_id,lat,lng,date,time,altitude,course,speed,humidity,temperature,pm1,pm2.5,pm10
 */
app.post('/api/v0/air.csv', (req, res) => {
  console.log("Posting AIR data ...");
  console.log(req.body);
  var csv = req.body;
  var rows = csv.split("\n").map(r => r.split(','));
  var data = {};
  console.log(rows);
  rows.forEach((r) => {
    fields.forEach((f, i) => {
      data[f] = fieldTypes[f](r[i]) || null;
    });
  });

  post('air', data);
  res.send({ ok: 1 });
});

app.get('/api/v0/air', (req, res) => {
  get('air', (err, json) => {
    if(err) { return res.send(500) }
    res.send(json);
  });
});

app.get('/api/v0/air/:sensor_id', (req, res) => {
  get('air', req.params.sensor_id, (err, json) => {
    if(err) { return res.send(500) }
    res.send(json);
  });
});

app.listen(3000, () => {
  console.log('App listening on port 3000!')
})
