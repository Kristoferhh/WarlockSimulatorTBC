class SimWorker {
	constructor(simulationEnd, simulationUpdate) {
		this.worker = new Worker('./src/js/web_worker.js');
		this.worker.onmessage = function(e) {
			if (e.data.event == "end") {
				simulationEnd(e.data.data);
			} else if (e.data.event == "update") {
				simulationUpdate(e.data.data);
			}
		}
	}

	start(workerParams) {
		this.worker.postMessage(workerParams);
	}
}