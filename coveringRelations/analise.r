
analise = function(iterations, prefix ="", Debug = FALSE){
	file_name = paste(prefix,"result_" , iterations, ".txt" , sep="")
	data <- read.table(file_name)
	if(Debug)
		print(data)
	nonnans <- which(data[,2] != "NaN")
	if(Debug)
		print(data[nonnans,])
	
	rbind(data,data[dim(data)[2],])
	t = length(nonnans)
	z = sapply(1:t, function(x) {
		 data[nonnans[x]+1,2] != "NaN" && data[nonnans[x],2] * data[nonnans[x]+1,2] <= 0
	})
	
	#print(z)
	#print(nonnans[which(z),])
	data[nonnans[which(z)],]
}

getPossible = function(possprefix = "", resultprefix="", Debug = FALSE){
	for(i in 1:7){
		file_name = paste(possprefix,"poss_", i, ".txt",sep="")
		poss = analise(i,resultprefix, Debug)
		row.names(poss) <- NULL
		write.table(x = poss,file=file_name,row.names=FALSE)
		#print(class(analise(i))
	}
}

uniqueAndNoNans = function(input_prefix="poss/periodic_", output_prefix="poss/p_",Debug = FALSE){
	for(i in 1:5){
		input_file = paste(input_prefix,i,".txt",sep="")
		output_file = paste(output_prefix,i,".txt",sep="")
		data <- read.table(input_file)
		write.table(x = unique(data[which(data!="NaN"),1]),file = output_file,row.names=FALSE)
	}
}

