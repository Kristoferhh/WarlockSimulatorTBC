class SimWorker {
	constructor(simulationEnd) {
		this.worker = new Worker('./src/js/web_worker.js');
		this.worker.onmessage = function(e) {
			if (e.data.event == "end") {
				simulationEnd(e.data.data);
			}
		}
	}

	start(workerParams) {
		this.worker.postMessage(workerParams);
	}
}