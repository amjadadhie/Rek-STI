"use client"

import type { NextPage } from 'next';
import Card from '../components/Card';
import Footer from '../components/footer';
import Navbar from '../components/navbar';
import { useEffect, useState } from 'react'
import supabase from '../supabaseClient'

type Data = {
  humidity: string;
  ph: string;
  uv: string;
}

const Home: NextPage = () => {
  const [data, setData] = useState<Data>()
  const [loading, setLoading] = useState<boolean>(true)


  
  useEffect(() => {
    const fetchData = async () => {
      const { data, error } = await supabase
        .from('data_iot')
        .select('humidity,uv,ph')
        .order('created_at', { ascending: false })
        .limit(1)
        .single(); // This ensures you get a single object instead of an array

      if (error) {
        console.error('Error fetching data:', error);
      } else {
        setData(data);
      }
      setLoading(false);
    };

    fetchData();
  }, []);

  const moistureStatus = getMoistureStatus(data?.humidity as unknown as number);
  const phStatus = getPhStatus(data?.ph as unknown as number);
  const uvStatus = getUvStatus(data?.uv as unknown as number/10);

  return (
    <div className="max-w-7xl mx-auto py-8 px-4 sm:px-6 lg:px-8">
      <Navbar />
      <div className="mb-8 text-center">
        <h1 className="text-2xl font-bold text-green-600">Sehat</h1>
        <p>Kondisi tanaman saat ini optimal</p>
      </div>
      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-8 mb-10">
        <Card
          isPh = {false}
          isUv={false}
          progress={data?.humidity as unknown as number ?? 0} // Placeholder, update with actual logic
          title="Kelembapan Tanah"
          status={moistureStatus.status}
          description={moistureStatus.description}
          recommendation= {moistureStatus.recommendation}
          />
        <Card
        isPh = {true}
        isUv={false}
          progress={data?.ph as unknown as number ?? 0} // Placeholder, update with actual logic
          title="pH Tanah"
          status={phStatus.status}
          description={phStatus.description}
          recommendation={phStatus.recommendation}
        />
        <Card
        isUv={true}
        isPh = {false}
          progress={(data?.uv as  unknown as number)/10 ?? 0} // Placeholder, update with actual logic
          title="Index UV"
          status={uvStatus.status}
          description={uvStatus.description}
          recommendation={uvStatus.recommendation}
          />
      </div>
      <Footer />
    </div>

  );
};

export default Home;

function getMoistureStatus(humidity: number){
  if (humidity < 30) {
    return {
      status: "Sangat Rendah",
      description: "Kelembapan tanah sangat rendah (< 30%). Ini menunjukkan bahwa tanah sangat kering dan membutuhkan perhatian segera.",
      recommendation: "Segera lakukan penyiraman untuk meningkatkan kelembapan tanah. Pastikan tanah mendapatkan air yang cukup untuk mencapai setidaknya 50%."
    };
  } else if (humidity < 40) {
    return {
      status: "Rendah",
      description: "Kelembapan tanah berada di bawah kisaran normal (30% - 40%). Tanah mulai mengering dan membutuhkan perhatian.",
      recommendation: "Lakukan penyiraman untuk meningkatkan kelembapan tanah. Pastikan tanah mendapatkan air yang cukup untuk mencapai setidaknya 50%."
    };
  } else if (humidity <= 60) {
    return {
      status: "Normal",
      description: "Kelembapan tanah dalam kisaran normal (40% - 60%). Ini menunjukkan kondisi tanah yang baik untuk pertumbuhan tanaman.",
      recommendation: "Tidak diperlukan tindakan khusus. Pertahankan penyiraman rutin untuk menjaga kelembapan tanah."
    };
  } else {
    return {
      status: "Tinggi",
      description: "Kelembapan tanah di atas kisaran normal (> 60%). Ini menunjukkan bahwa tanah terlalu basah.",
      recommendation: "Kurangi frekuensi penyiraman untuk mencegah kelembapan berlebihan yang dapat menyebabkan penyakit tanaman."
    };
  }
};

function getUvStatus(uv: number) {
  if (uv < 1) {
    return {
      status: "Rendah",
      description: "Indeks UV sangat rendah (< 1). Ini menunjukkan kondisi yang kurang optimal untuk fotosintesis.",
      recommendation: "Pastikan tanaman mendapatkan paparan cahaya yang cukup sepanjang hari."
    };
  } else if (uv <= 8) {
    return {
      status: "Normal",
      description: "Indeks UV dalam kisaran normal (1 - 8). Ini menunjukkan kondisi yang baik untuk fotosintesis.",
      recommendation: "Tidak diperlukan tindakan khusus. Pertahankan paparan cahaya yang cukup untuk tanaman."
    };
  } else {
    return {
      status: "Tinggi",
      description: "Indeks UV tinggi (> 8). Ini menunjukkan kondisi yang mungkin terlalu terik untuk beberapa tanaman.",
      recommendation: "Berikan naungan atau kurangi paparan sinar matahari langsung selama periode terik untuk melindungi tanaman dari kerusakan."
    };
  }
}

function getPhStatus(ph: number) {
  if (ph < 5.5) {
    return {
      status: "Asam",
      description: "Nilai pH tanah sangat rendah (< 5.5). Ini menunjukkan kondisi tanah yang sangat asam.",
      recommendation: "Tambahkan kapur pertanian untuk meningkatkan pH tanah dan mencapai kisaran optimal."
    };
  } else if (ph < 6.0) {
    return {
      status: "Sedikit Asam",
      description: "Nilai pH tanah rendah (5.5 - 6.0). Tanah sedikit asam.",
      recommendation: "Monitor pH tanah dan tambahkan kapur pertanian jika diperlukan."
    };
  } else if (ph <= 7.0) {
    return {
      status: "Optimal",
      description: "Nilai pH tanah dalam kisaran normal (6.0 - 7.0). Ini menunjukkan kondisi tanah yang baik untuk pertumbuhan tanaman.",
      recommendation: "Tidak diperlukan tindakan khusus. Pertahankan pemantauan rutin untuk memastikan stabilitas pH tanah."
    };
  } else {
    return {
      status: "Alkalin",
      description: "Nilai pH tanah tinggi (> 7.0). Tanah sedikit alkalin.",
      recommendation: "Tambahkan sulfur pertanian untuk menurunkan pH tanah dan mencapai kisaran optimal."
    };
  }
}
