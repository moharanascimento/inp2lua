-------------------------------------------------------------
--  Post process Gid script
--  Autor: Julio Rueda
-------------------------------------------------------------
-------------------------------------------------------------
--  Adaptative Time function 
-- Autor: Julio Rueda
-------------------------------------------------------------
function adaptativeTime(dt,err,conv,niter,nbroke,solverOptions)
  --local hydtol = 
  local TolPhy  = solverOptions.tolerance["mechanic"] 
  local ftime = 2
  local dtol = (TolPhy-err)/TolPhy*100
  if(conv)then
      if(dtol < 15 or niter > 5) then
        dt = dt
      elseif(ftime*dt >= solverOptions.timeMaxIncrement ) then
        dt = solverOptions.timeMaxIncrement 
      elseif(dtol > 0 and dtol < 20) then
        dt = 1.2*dt 
      else
        dt = ftime*dt        
      end 
   else
      if(nbroke - 1 < solverOptions.attemptMax) then
        dt = dt/4
      elseif(nbroke > solverOptions.attemptMax)then
        dt = solverOptions.timeMinIncrement
      end  
   end
   --io.print(tr('DTol = %1 %') :num(dtol))      
   return dt
end
