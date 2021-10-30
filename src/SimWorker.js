class SimWorker {
  constructor (dpsUpdate, combatLogVector, errorCallback, combatLogUpdate, combatLogBreakdown, simulationEnd, simulationUpdate, workerParams) {
    this.worker = new Worker(`../${process.env.PUBLIC_URL}/web.worker.js`);
    this.workerParams = workerParams;
    this.worker.onmessage = function (event) {
      const data = event.data.data;
      switch(event.data.event) {
        case 'dpsUpdate':
          dpsUpdate(data);
          break;
        case 'errorCallback':
          errorCallback(data);
          break;
        case 'combatLogUpdate':
          combatLogUpdate(data);
          break;
        case 'end':
          simulationEnd(data);
          break;
        case 'update':
          simulationUpdate(data);
          break;
        case 'combatLogBreakdown':
          combatLogBreakdown(data);
          break;
        case 'combatLogVector':
          combatLogVector(data);
          break;
        default:
          return;
      }
    }
  }

  start () {
    this.worker.postMessage(this.workerParams);
  }
}

export { SimWorker }