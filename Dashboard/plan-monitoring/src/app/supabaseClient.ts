// supabaseClient.ts
import { createClient } from '@supabase/supabase-js'

const SUPABASE_URL = 'https://zohwotklgcwnxhrvjdbc.supabase.co'
const SUPABASE_KEY = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InpvaHdvdGtsZ2N3bnhocnZqZGJjIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MTc0MzA3MDYsImV4cCI6MjAzMzAwNjcwNn0.qjhyxaCesSBL4QU-SOaSbhCJX-iWG3mwGv4zNOCYlrk'

const supabase = createClient(SUPABASE_URL, SUPABASE_KEY)

export default supabase
