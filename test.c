int main() {
	int i, j;
	j = 3;
	for(i = 0;i < 5;i++){
		j = j*i + i;
		if(j == 15)
			break;
	}
	return 0;
}
