class SimWorker {
  constructor (dpsUpdate, combatLogVector, errorCallback, combatLogUpdate, combatLogBreakdown, simulationEnd, simulationUpdate, workerParams) {
    this.worker = new Worker('./src/js/web_worker.js')
    this.workerParams = workerParams
    this.worker.onmessage = function (e) {
      if (e.data.event == "dpsUpdate") {
        dpsUpdate(e.data.data)
      } else if (e.data.event == "errorCallback") {
        errorCallback(e.data.data)
      } else if (e.data.event == "combatLogUpdate") {
        combatLogUpdate(e.data.data)
      } else if (e.data.event == 'end') {
        simulationEnd(e.data.data)
      } else if (e.data.event == 'update') {
        simulationUpdate(e.data.data)
      } else if (e.data.event == "combatLogBreakdown") {
        combatLogBreakdown(e.data.data)
      } else if (e.data.event == "combatLogVector") {
        combatLogVector(e.data.data)
      }
    }
  }

  start () {
    this.worker.postMessage(this.workerParams)
  }
}
