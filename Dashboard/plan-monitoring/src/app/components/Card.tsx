import { FC } from 'react';
import { CircularProgressbar, buildStyles } from 'react-circular-progressbar';
import 'react-circular-progressbar/dist/styles.css';

interface CardProps {
  title: string;
  description: string;
  status: string;
  isPh: boolean;
  isUv: boolean;
  recommendation: string;
  progress: number; // New prop for progress value
}

const Card: FC<CardProps> = ({ title, status, description, recommendation, progress, isPh, isUv }) => {
  return (
    <div className="bg-white shadow-md rounded-lg p-6">
      <h3 className="text-lg font-bold text-green-600">{title}</h3>
      <div className="flex justify-center my-4">
        <CircularProgressbar 
          value={isUv ? progress : progress} maxValue={isUv ? 10 : isPh ? 14 : 100}
          text={`${progress}`} 
          styles={buildStyles({
            textColor: "#10B981",
            pathColor: "#10B981",
            trailColor: "#D1D5DB"
          })}
        />
      </div>
      <div className="text-center my-4">
      </div>
      <div className="my-4">
        <h4 className="font-semibold text-gray-700">{status}</h4>
        <p className="text-gray-600">{description}</p>
      </div>
      <div className="my-4">
        <h4 className="font-semibold text-gray-700">Rekomendasi</h4>
        <p className="text-gray-600">{recommendation}</p>
      </div>
    </div>
  );
};

export default Card;
