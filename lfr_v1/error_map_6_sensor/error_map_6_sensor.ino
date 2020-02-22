void error_map(){
  if((s3==sc)&&(s4==lc)&&(s5==lc)&&(s6==sc))
    error=0;
  else if((s3==lc)&&(s4==lc)&&(s5==lc)&&(s6==sc))
    error=-0.5;
  else if((s3==sc)&&(s4==lc)&&(s5==sc)&&(s6==sc))
    error=-0.5;
  else if((s3==sc)&&(s4==lc)&&(s5==lc)&&(s6==lc)) 
   error=0.5;
  else if((s3==sc)&&(s4==sc)&&(s5==lc)&&(s6==sc))
   error=0.5;
  
  else if((s2==sc)&&(s3==lc)&&(s4==lc)&&(s5==sc))
   error=-1; 
  else if((s4==sc)&&(s5==lc)&&(s6==lc)&&(s7==sc))
   error=1;
  
  else if((s2==lc)&&(s3==lc)&&(s4==lc)&&(s5==sc))
   error=-1.5;
  else if((s2==sc)&&(s3==lc)&&(s4==sc)&&(s5==sc))
   error=-1.5; 
  else if((s4==sc)&&(s5==lc)&&(s6==lc)&&(s7==lc))
   error=1.5;
  else if((s4==sc)&&(s5==sc)&&(s6==lc)&&(s7==sc))
   error=1.5;
   
  else if((s2==lc)&&(s3==lc)&&(s4==sc))
   error=-2;
  else if((s5==sc)&&(s6==lc)&&(s7==lc))
   error=2;
      
  if((s2==lc)&&(s2==sc)&&(s7==sc))
   error=-2.5; 
  if((s2==sc)&&(s7==sc)&&(s7==lc))
   error=2.5;
}

