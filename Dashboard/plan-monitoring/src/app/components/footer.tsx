import Link from 'next/link';
import Image from 'next/image';
import { FC } from 'react';

const Footer: FC = () => {
    return (
      <footer className="bg-green-800 text-white py-8">
        <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 grid grid-cols-1 md:grid-cols-3 gap-8">
          <div>
            <h2 className="text-lg font-bold mb-4">GreenAlert</h2>
            <p>Copyright © greenalert 2024 All rights reserved</p>
          </div>
          <div>
            <h2 className="text-lg font-bold mb-4">Menu</h2>
            <ul>
              <li><a href="/" className="hover:underline">Home</a></li>
              <li><a href="/about" className="hover:underline">Tentang Kami</a></li>
              <li><a href="/data" className="hover:underline">Real-Time Data</a></li>
              <li><a href="/contact" className="hover:underline">Kontak Kami</a></li>
            </ul>
          </div>
          <div>
            <h2 className="text-lg font-bold mb-4">Contact Info</h2>
            <p>+123 456 789</p>
            <p>support@greenalert.com</p>
            <p>Indonesia</p>
          </div>
        </div>
        <div className="mt-8 text-center">
          <a href="#" className="mx-2 text-white hover:text-gray-400">Facebook</a>
          <a href="#" className="mx-2 text-white hover:text-gray-400">Twitter</a>
          <a href="#" className="mx-2 text-white hover:text-gray-400">Instagram</a>
        </div>
      </footer>
    );
  };
  
  export default Footer;
  