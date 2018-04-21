import {Component} from '@angular/core';
import {LoadingController} from 'ionic-angular';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  public infoList = [
    {title: 'Temperature', id: 'temp', symbol: 'C'},
    {title: 'Program completed', id: 'completed', symbol: '%'}
  ];
  public settingsList = [
    {title: 'Choke angle', id: 'chokeAngle', symbol: 'deg'},
    {title: 'HH angle', id: 'hhAngle', symbol: 'deg'},
    {title: 'Hot time', id: 'hotTime', symbol: 'sec'},
  ];
  public isSensorWork: boolean;

  constructor(public loadingCtrl: LoadingController) {
  }

  connectToBT(): void {
    let loader = this.loadingCtrl.create({
      content: "Search BT! Please wait...",
      duration: 3000
    });
    loader.present();
  }

  setParam(fieldName: string): void {
    console.log('Set ', fieldName)
  }

}
