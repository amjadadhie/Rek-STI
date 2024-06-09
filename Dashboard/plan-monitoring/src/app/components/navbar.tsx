import Link from 'next/link';
import Image from 'next/image';
import { FC } from 'react';

const Navbar: FC = () => {
  return (
    <header className="bg-white shadow-sm">
      <div className="max-w-7xl mx-auto py-4 px-4 sm:px-6 lg:px-8 flex items-center justify-between">
        <div className="flex items-center">
          {/* <Image src="/logo.png" alt="GreenAlert Logo" width={40} height={40} /> */}
          <span className="ml-3 text-xl font-bold text-green-600">GreenAlert</span>
        </div>
        <nav className="flex space-x-4">
          <Link href="/" className="text-gray-600 hover:text-green-600">Beranda</Link>
          <Link href="/about" className="text-gray-600 hover:text-green-600">Tentang Kami</Link>
          <Link href="/data" className="text-gray-600 hover:text-green-600">Real-Time Data</Link>
          <Link href="/contact" className="text-gray-600 hover:text-green-600">Kontak Kami</Link>
        </nav>
        <div className="flex items-center space-x-4">
          {/* <Image src="/avatar.png" alt="User Avatar" width={30} height={30} /> */}
          <span className="text-gray-600">Olivia</span>
        </div>
      </div>
    </header>
  );
};

export default Navbar;
