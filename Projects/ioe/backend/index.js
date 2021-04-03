const io = require("socket.io-client");
const socket = io("http://localhost:3001/");

const q = 'testq';

function bail(err) {
    console.error(err);
    process.exit(1);
}

// Consumer
function consumer(conn) {
    var ok = conn.createChannel(on_open);
    function on_open(err, ch) {
        if (err != null) bail(err);
        ch.assertQueue(q);
        ch.consume(q, function (msg) {
            if (msg !== null) {
                console.log(msg.content.toString());
                switch (JSON.parse(msg.content.toString()).content[0]) {
                    case "A":
                        socket.emit("key", "a");
                        break;
                    case "9":
                        socket.emit("key", "b");
                        break;
                    case "C":
                        socket.emit("key", "c");
                        break;
                }
                ch.ack(msg);
            }
        });
    }
}

require('amqplib/callback_api')
    .connect('amqps://ylhnwvqf:U0gpsudyXQBu8LiwAIOc7um4Av9ajPBd@lion.rmq.cloudamqp.com/ylhnwvqf', function (err, conn) {
        if (err != null) bail(err);
        consumer(conn);
    });