void test()
{
  TH1F* hParticleTypes =
      new TH1F("hParticleTypes", "Generated particle types", 7, 0, 42);
  hParticleTypes->Fill(0);
  hParticleTypes->Fill(0);
  hParticleTypes->Fill(0);
  hParticleTypes->Fill(1);
  hParticleTypes->Fill(1);
  hParticleTypes->Fill(3);
  hParticleTypes->Fill(5);
  hParticleTypes->Fill(4);
  hParticleTypes->Fill(6);
  hParticleTypes->Fill(6);

  hParticleTypes->Draw();
}