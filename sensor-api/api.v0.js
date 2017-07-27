const mongoose = require('mongoose');
mongoose.connect('mongodb://unloquer-mongo/sensors');

module.exports = { post, get };

var AirPost = mongoose.model('AirPost', AirPostSchema());

function post(model, data) {
  var Post = {
    air: AirPost
  }[model];

  var p = new Post(data);
  p.save((err) => {
    if(err) { console.err(err); }
  });
}

function get() {
  AirPost.find((err, data) => {
    err && console.log(err);
    data && console.log(data);
  });
}

function AirPostSchema() {
  return {
    "lat": Number,
    "lng": Number,
    "date": String,
    "time": String,
    "timestamp": Date,
    "altitude": Number,
    "course": Number,
    "speed": Number,
    "humidity": Number,
    "temperature": Number,
    "pm1": Number,
    "pm25": Number,
    "pm10": Number
  }
}
