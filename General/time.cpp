  auto start = chrono::high_resolution_clock::now();
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> elapsed = end - start;
  cerr << "Time taken to compute : " << elapsed.count() << " seconds" << endl;