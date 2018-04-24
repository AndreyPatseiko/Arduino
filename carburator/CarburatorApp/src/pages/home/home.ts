import {Component, NgZone} from '@angular/core';
import {LoadingController} from 'ionic-angular';
import {BluetoothSerial} from '@ionic-native/bluetooth-serial';


@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  public zone = new NgZone({enableLongStackTrace: false});
  public infoList = [
    {title: 'Temperature', id: 'temp', symbol: 'C', paramsName: 'temperature'},
    {title: 'Program completed', id: 'completed', symbol: '%', paramsName: 'complited'}
  ];
  public settingsList = [
    {title: 'Choke angle', id: 'chokeAngle', symbol: 'deg'},
    {title: 'HH angle', id: 'hhAngle', symbol: 'deg'},
    {title: 'Hot time', id: 'hotTime', symbol: 'sec'},
  ];
  private enableBTLoader;
  public isSensorWork: boolean;
  public isSetupMode: boolean;
  public info: any

  // public info: { temperature: number, complited: number };

  constructor(private loadingCtrl: LoadingController,
              private bluetoothSerial: BluetoothSerial) {
  }

  connectForInsecure(data): void {
    this.bluetoothSerial.connectInsecure(data.address).subscribe(
      () => {
        console.log('Connection success');
        this.subscribeReadData();
        this.enableBTLoader.present()
      },
      err => {
        console.log('Connection error', err.message)
      }
    );
  }

  connectToBT(): void {
    this.enableBTLoader = this.loadingCtrl.create({
      content: "Connect...",
      duration: 3000
    });
    this.bluetoothSerial.enable().then(
      () => {
        this.connectForInsecure({address: '98:D3:31:FD:28:7E'})

      }
    );
  }

  setParam(fieldName: string): void {
    console.log('Set ', fieldName)
  }

  setupModeChange(): void {
    this.isSetupMode = !this.isSetupMode;
    this.bluetoothSerial.write(this.isSetupMode ? '1' : '0');
  }

  subscribeReadData(): void {
    this.bluetoothSerial.subscribe('|').subscribe(
      res => {
        this.zone.run(() => {
          this.info = JSON.parse(res.slice(0, -1));
        });
      });


  }

}
