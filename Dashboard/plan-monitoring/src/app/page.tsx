import Link from "next/link";
import { redirect } from "next/navigation";

export default function Page() {
  redirect("/home");
  return <main className="bg-[#EFF6FD]"></main>;
}
