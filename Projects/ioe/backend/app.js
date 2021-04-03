const express = require('express');
const cors = require('cors');
const path = require('path');
const app = express();

app.use(cors());

app.use(express.static('public'))

app.get("/", express.static(path.join(__dirname, 'public')));

let server = app.listen(3001);
console.log("listening on 3001");

const io = require('socket.io')(server);

io.on('connection', (socket) => {
    console.log('someone connected');
    socket.on('key', (res) => {
        console.log(res);
        io.emit('ps', res);
    });
    socket.on('disconnect', () => {
        console.log('someone disconnected');
    });
});