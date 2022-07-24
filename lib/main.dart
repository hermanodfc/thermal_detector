// Copyright (C) 2022 Hermano Costa

// This project was inspired by https://github.com/X-dea/SnakeEye


// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

import 'package:flutter/material.dart';
import 'package:usb_serial/usb_serial.dart';

import 'common.dart';
import 'configuration.dart';
import 'sensor.dart';

void main() {
  runApp(const App());
  initFFI();
}

class App extends StatelessWidget {
  const App({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Thermal Viewer',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const MainPage(),
    );
  }
}

class MainPage extends StatefulWidget {
  const MainPage({Key? key}) : super(key: key);

  @override
  _MainPageState createState() => _MainPageState();
}

class _MainPageState extends State<MainPage> {
  var controller = TextEditingController(text: '192.168.4.1');
  var devices = <UsbDevice>[];
  var cameraPreview = false;
  var scale = 1;

  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Connection'),
      ),
      body: ListView(
        children: [
          Padding(
            padding: const EdgeInsets.all(10.0),
            child: TextField(
              controller: controller,
              decoration: const InputDecoration(
                border: OutlineInputBorder(),
                labelText: 'Address',
              ),
            ),
          ),
          Wrap(
            alignment: WrapAlignment.spaceEvenly,
            spacing: 10,
            runSpacing: 10,
            children: [
              ElevatedButton(
                child: const Text('Connect'),
                onPressed: () => Navigator.of(context).push(
                  MaterialPageRoute(
                    builder: (context) => SensorPage(
                      address: controller.text,
                      scale: scale,
                    ),
                  ),
                ),
              ),
              ElevatedButton(
                child: const Text('Configure Sensor'),
                onPressed: () {
                  final address = controller.text;
                  if (address.startsWith('serial://')) return;
                  showDialog(
                    context: context,
                    builder: (context) => SensorConfigurationDialog(
                      address: address,
                    ),
                  );
                },
              ),
            ],
          ),
        ],
      ),
    );
  }
}
