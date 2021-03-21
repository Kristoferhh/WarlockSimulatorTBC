class SimWorker {
	constructor(simulationEnd, simulationUpdate, workerParams) {
		this.worker = new Worker('./src/js/web_worker.js');
		this.workerParams = workerParams;
		this.worker.onmessage = function(e) {
			if (e.data.event == "end") {
				simulationEnd(e.data.data);
			} else if (e.data.event == "update") {
				simulationUpdate(e.data.data);
			}
		}
	}

	start() {
		//alert(JSON.stringify(this.workerParams));
		this.worker.postMessage(this.workerParams);
	}
}