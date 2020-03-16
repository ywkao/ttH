### Sam's Shell Utility Functions ###


### Function to run tasks in parallel. Poor man's version of condor.    ###
### That's why it's called pigeon (poor pigeons)                        ###
### Arg1: name of executable                                            ###
### Arg2: args to be passed to executable (NOTE: must put quotes around ###
###       it if there is more than one, e.g. "$args"                    ###       
### Arg3: number of jobs to run in parallel                             ###
### Arg4: (OPTIONAL) niceness -- default is 19 (maximally nice)         ###
function pigeon_submit
{
  if [ -z "$nCur" ]; then
    nCur=0
  fi
  if [ -z "$pid" ]; then
    pid=('')
  fi

  if (( $# == 4 )); then
    niceness=$4
  else
    niceness=19
  fi
  #echo "[PIGEON]: Executable is "$1
  #echo "[PIGEON]: Args are "$2
  while (( $nCur >= $3  ))
    do
      idxT=0
      sleep .1
      while (( $idxT < $nPar ))
      do
        if [ -e /proc/${pid[$idxT]} ]
        then
          let "idxT++"
          continue
        else
          let "nCur--"
          #echo "[PIGEON]: Task no. "$idxT" finished."
          break
        fi
      done
    done
  #echo "[PIGEON]: Submitting another job since "$idxT" just finished."
  timeout 60m /bin/nice -n $niceness ./$1 $2 &
  pid[$idxT]=$!
  exitValue=$?
  let "nCur++"
  let "idxT++"
  return "$exitValue"
}


### Function to add large numbers (>100) of histograms using hadd 		###
### hadd seems to have a limit on the number of histograms you can give it, 	###
### this adds histos in groups of 30 so you can sleep easy knowing that hadd 	###
### hasn't skipped over any of your well-deserved histograms 			###
### Arg1: name of output histogram (e.g. "histosMaster") 			###
### Arg2: name of input histogram (e.g. "histosToAdd_4.root" would be input 	###
### 	  as "histosToAdd"							###
### Arg3: number of input histograms of the above form 				###
### Arg4: (OPTIONAL) number of cores to run on (default 1) 			###
function addHistos 
{
  if (( $# < 4 ))
  then
    nPar=1
  else
    nPar=$4
  fi

  histosToAdd=""
  bigHistos=""
  idx1=1
  idx2=1
  while (($idx1 <= $3))
  do
    for ((i=1; i<=30; i++))
    do
      if (($idx1 <= $3))
      then
        if [ -e $2"_"$idx1".root" ]; then 
          histosToAdd=$histosToAdd" "$2"_"$idx1".root"
        fi
        let "idx1++"
      fi
    done
    hadd -f -k $1"_"$idx2".root" $histosToAdd &
    if (($idx2 % $nPar == 0))
    then
      wait
    fi
    bigHistos=$bigHistos" "$1"_"$idx2".root"
    histosToAdd=""
    let "idx2++"
  done
  wait
  #hadd -f -k $1".root" $bigHistos
}
