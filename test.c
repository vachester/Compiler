int main() {
	int i ;
	int A[3] ;
	bool flag;

	for(i = 0;i < 3;i++){
		int tmp;
		tmp = i + 4*(10-i);
		A[i] = tmp;
	}
	
	flag = true;
	while(flag) {
		A[1] = A[0];
		A[2] = A[1];
	}

	
	if(A[0] > 0 && A[1] < 0)
		A[0] ++;
	else {
		A[0] --;
		A[1] ++;
	}

	return 0;
}




