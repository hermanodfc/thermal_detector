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

import 'dart:async';
import 'dart:io';
import 'package:ext_storage/ext_storage.dart';
import 'package:intl/intl.dart';
import 'package:permission_handler/permission_handler.dart';

class ThermalStorage {

  Future<String> get _localPath async {
    final directory = await ExtStorage.getExternalStoragePublicDirectory(
        ExtStorage.DIRECTORY_DOWNLOADS);
    return directory;
  }

  void writeCounter(String thermalData) async {
    if (await Permission.manageExternalStorage.request().isGranted) {
      String directory = await ExtStorage.getExternalStoragePublicDirectory(
          ExtStorage.DIRECTORY_DOCUMENTS);
      String fileName = DateFormat("yyyy-MM-dd HH-mm-ss S").format(
          DateTime.now());
      File fileDef = await File('$directory/thermal/' + fileName + '.txt').create(
          recursive: true);
      fileDef.writeAsStringSync(thermalData);
    }
  }
}