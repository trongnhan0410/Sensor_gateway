# Sensor Monitoring System

Introduction

The Sensor Monitoring System consists of multiple sensor nodes measuring room temperature and transmitting data to a central sensor gateway. The sensor gateway collects, processes, and stores the data in an SQLite database. Communication between sensor nodes and the gateway is established through a TCP connection. The system is designed to handle dynamic connections and process data efficiently using multithreading.

System Architecture

The system consists of:

Sensor Nodes: Simulated software entities that send temperature data to the gateway over TCP.

Sensor Gateway: The core processing unit that receives, analyzes, logs, and stores the data.

SQLite Database: Stores all sensor data for further analysis.

Components

Connection Manager: Listens for new sensor connections, receives data, and writes it to shared memory.

Data Manager: Analyzes sensor data, calculates average temperature, and determines if the environment is too hot or too cold.

Storage Manager: Saves sensor data to the SQLite database and handles reconnection attempts in case of failures.

Log Process: Records system events in a log file through an inter-process communication mechanism.
Minimal Requirements

Req 1

The sensor gateway consists of a main process and a log process.

The log process is started using fork() as a child process of the main process.

Req 2

The main process runs three threads: connection, data, and storage manager.

A shared data structure is used for communication between threads and must be thread-safe.

Req 3

The connection manager listens on a TCP socket for incoming connections from new sensor nodes.

The port number is provided as a command-line argument, e.g., ./server 1234.

Req 4

The connection manager captures incoming packets and writes the data to the shared structure.

Req 5

The data manager reads sensor measurements, computes a running average, and determines if the temperature is too hot or cold.

It does not write running averages to shared data; these values are for internal decision-making.

Req 6

The storage manager retrieves sensor data and inserts it into the SQLite database.

If the database connection fails, it retries up to three times before closing the gateway.

Req 7

The log process receives log events via a FIFO queue (logFifo).

If the FIFO does not exist at startup, it will be created dynamically.

The FIFO must be thread-safe.

Req 8

Log events contain ASCII information describing event types.

Each log event follows the format: <sequence_number> <timestamp> <log-event message> and is stored in gateway.log.

Req 9

The system must log the following events:

From the connection manager:

Sensor node <SensorNodeID> opened a new connection.

Sensor node <SensorNodeID> closed the connection.

From the data manager:

Sensor node <SensorNodeID> reports temperature as too cold (avg temperature = X).

Sensor node <SensorNodeID> reports temperature as too hot (avg temperature = X).

Received sensor data with an invalid sensor ID.

From the storage manager:

Connection to SQL server established.

New table <table_name> created.

Connection to SQL server lost.

Unable to connect to SQL server.
