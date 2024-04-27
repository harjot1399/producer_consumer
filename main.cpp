#include "main.h"

void Trans( int n ) {
	long i, j;

	// Use CPU cycles 
	j = 0;
	for( i = 0; i < n * 100000; i++ ) {
		j += i ^ (i+1) % (i+n);
	}
	TransSave += j;
	TransSave &= 0xff;
}


// Sleep simulates pauses between transactions
// Parameter given expresses wait time in hundreds of a nanosecond.

void Sleep( int n ) {
	struct timespec sleep;

	// Make sure pass a valid nanosecond time to nanosleep
	if( n <= 0 || n >= 100 ) {
		n = 1;
	}

	// Sleep for less than one second
	sleep.tv_sec  = 0;
        sleep.tv_nsec = n * 10000000 + TransSave;
	if( nanosleep( &sleep, NULL ) < 0 ) {
		perror ("NanoSleep" );
	}
}

// Function which give time from start of the program to a particular instance in program in microseconds
double gettime(){
    double s_to_micro = 1000000;
    return (double)duration_cast<microseconds>(high_resolution_clock::now() - start).count() / s_to_micro;
}

// Function to get the size of the buffer
int get_buffer_size(){
    sem_wait(&sem_mutex);
    int s = (int)buffer.size();
    sem_post(&sem_mutex);
    return s;

}

// Function to log every activity to the output file
void log_activity (int id, string str, int n){
    // the following if else if  structure which command was recieved and increment thier counter variable accordingly
    if (str == "Ask"){
        askcount++;
    }else if(str == "Recieve"){
        recievecount++;
    }else if(str == "Complete"){
        completecount++;
    }else if(str == "Sleep"){
        sleepcount++;
    }else if(str == "Work"){
        workcount++;
    }
    double duration = gettime();
    // In this the variable start is declared globally the first time instance is 0.001 so subtracting 0.001 to get 0.000
    if (duration >= 0.001){
        duration = duration - 0.001;
    }
    sem_wait(&sem_outfile);
    if (str == "Work" || str == "Recieve"){
        int buffer_size = get_buffer_size();
        fprintf(stdout,"%8.3f ID= %u Q= %u %-10s%d\n",duration,id,buffer_size,str.c_str(),n);
    }else if(str == "Ask" || str == "End"){
        fprintf(stdout,"%8.3f ID= %-6u %s\n",duration,id,str.c_str());
    }else{
        fprintf(stdout,"%8.3f ID= %-6u %-10s%d\n",duration,id,str.c_str(),n);
    }
    sem_post(&sem_outfile);
}

// Start routine function for all the consumer threads
void *consume(void *args){
    int p = *(int *)args;
    while (true){
        log_activity(p+1, "Ask", 0);
        sem_wait(&semFull);
        sem_wait(&sem_mutex);
        // the following if condition check when the last transcation is removed from the buffer for transcation and break out of the loop to teminate the thread
        if (buffer.empty()){
            sem_post(&semFull);
            sem_post(&sem_mutex);
            // freeing up the dyanmic allocated memory
            free(args);
            break;
        }
        int command_time = buffer.front();
        buffer.pop();
        sem_post(&sem_mutex);
        sem_post(&semEmpty);
        log_activity(p+1, "Recieve", command_time);
        Trans(command_time);
        log_activity(p+1, "Complete", command_time);
        c[p] += 1;
    }
    return nullptr;
}

// function to print to the output summary
void summary() {
    fprintf(stdout,"Summary:\n");
    fprintf(stdout,"    %-10s %5d\n","Work",workcount);
    fprintf(stdout,"    %-10s %5d\n","Ask",askcount);
    fprintf(stdout,"    %-10s %5d\n","Recieve",recievecount);
    fprintf(stdout,"    %-10s %5d\n","Complete",completecount);
    fprintf(stdout,"    %-10s %5d\n","Sleep",sleepcount);
    for (int i = 0; i < n; i++) {
        fprintf(stdout,"    Thread %-7u %u\n",i+1,c[i]);
    }
    double duration = gettime();
    double trans_per_second = (double)workcount / duration;
    fprintf(stdout,"Transactions per second: %-5.2f",trans_per_second);
}

// producer of the function which push items to the buffer
void transcation(int n){
    log_activity(0,"Work",n);
    sem_wait(&semEmpty);
    sem_wait(&sem_mutex);
    buffer.push(n);
    sem_post(&sem_mutex);
    sem_post(&semFull);
}

// function to execute sleep command
void sleep(int n){
    log_activity(0,"Sleep",n);
    Sleep(n);
}

// this function determines which command needs to be performed
void activity(string s){
    if (s[0] == 'T'){
        int n = stoi(s.substr(1,s.length()));
        transcation(n);
    }else if (s[0] == 'S'){
        int n = stoi(s.substr(1,s.length()));
        sleep(n);
    }
}

// this function takes input for the program
void input(){
    string str;
    while (!cin.eof()){
        getline(cin, str);
        activity(str);
    }
    log_activity(0,"End",0);
    
}

int main (int argc, char* argv[] ){
    if (argc < 2){
        cout << "Please provide number of threads to be created" << endl;
        return 0;
    }
    // intializing all the semaphores for the program
    sem_init(&semFull,0,0);
    sem_init(&sem_mutex,0,1 );
    sem_init(&sem_outfile,0,1);
    int nthreads = atoi(argv[1]);
    n = nthreads;
    int max_buf_size =  2*nthreads;
    sem_init(&semEmpty,0,max_buf_size);
    int in[nthreads] = {0};
    c = in;
    if (argc == 2){
        outfile = "prodcon.log";
    }else if (argc >= 3){
        outfile =  "prodcon." + string(argv[2]) + ".log";
    }
    ptr = freopen(outfile.c_str(),"w",stdout);
    pthread_t th[nthreads];
    
    int i;
    // spawning customers
    for (i = 0; i < nthreads; i++){
        int*a = (int*) malloc(sizeof(int));
        *a = i;
        pthread_create(&th[i],NULL,&consume,a);
    }
    input();
    // incrementing the semFull by one to unlock thread
    sem_post(&semFull);
    // terminating all the threads
    for (int i = 0; i < nthreads; i++){
        pthread_join(th[i],NULL);
    }
    summary();
    //closing the file
    fclose(ptr);
    //destroying the semaphores
    sem_destroy(&sem_mutex);
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    sem_destroy(&sem_outfile);
    return 0;
}