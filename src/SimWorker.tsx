import { WorkerParams } from "./Types";

interface ISimWorker {
  worker: Worker,
  workerParams: WorkerParams
}

export class SimWorker implements ISimWorker {
  worker;
  workerParams;  

  constructor (dpsUpdate: (data: any) => void, combatLogVector: (data: any) => void, errorCallback: (data: any) => void, combatLogUpdate: (data: any) => void
  , combatLogBreakdown: (data: any) => void, simulationEnd: (data: any) => void, simulationUpdate: (data: any) => void, workerParams: WorkerParams) {
    this.worker = new Worker('./src/js/web_worker.js');
    this.workerParams = workerParams;
    this.worker.onmessage = function (e: MessageEvent) {
      switch(e.data.event) {
        case 'dpsUpdate':
          dpsUpdate(e.data.data);
          break;
        case 'errorCallback':
          errorCallback(e.data.data);
          break;
        case 'combatLogUpdate':
          combatLogUpdate(e.data.data);
          break;
        case 'end':
          simulationEnd(e.data.data);
          break;
        case 'update':
          simulationUpdate(e.data.data);
          break;
        case 'combatLogBreakdown':
          combatLogBreakdown(e.data.data);
          break;
        case 'combatLogVector':
          combatLogVector(e.data.data);
          break;
      }
    }
  }

  start () {
    this.worker.postMessage(this.workerParams)
  }
}