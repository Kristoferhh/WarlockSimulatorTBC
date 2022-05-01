import { Bar } from "react-chartjs-2";
import { useSelector } from "react-redux";
import { RootState } from "../redux/Store";

export default function DpsHistogram() {
  const histogramState = useSelector((state: RootState) => state.ui.histogram);

  return histogramState.data ? (
    <Bar
      id="dps-histogram"
      style={{ display: histogramState.visible ? "" : "none" }}
      data={{
        labels: Object.keys(histogramState.data),
        datasets: [
          {
            label: "DPS Histogram",
            data: Object.keys(histogramState.data).map(
              (key) => histogramState.data![key]
            ),
            borderWidth: 1,
            borderColor: "#9482C9",
          },
        ],
      }}
    />
  ) : (
    <div></div>
  );
}
