
#if 0
/// tester
static void serial_test(void)
{
	int ret;
	int fd;
	char buf[32];

	fd = open_serial_port(global_conf.port, global_conf.speed);
	if ( fd < 0 ) {
		exit(1);
	}

	DBG("test start fd = %d\n", fd);

	while(1) {
		ret = read(fd, buf, sizeof(buf)-1);
		if ( ret < 0 ) {
			continue;
		}
		buf[ret] = 0;

		printf("%s:%d\n", buf, ret);
		if ( buf[0] == 'z') {
			break;
		}
	}

	close_serial_port();
}
#endif 

#if 0
int main(int argc, char *argv[])
{
	pthread_t receiver_thread;
	int fd_serial;
	char *raw_packet;
	int packet_size;
	struct cdr_packet packet;
	int ret;
	char buf[1024];
	unsigned char dump_packet[20][128];
	int dump_idx ;

	struct cdr_session session;

	if ( proc_cmd_options(argc, argv) < 0 ) {
		exit(1);
	}
	printf("CDR Mate for Linux V%s\n", VERSION_STR);

	if ( load_conf() < 0 ) {
		fprintf(stderr, "configuration file error.\n");
		exit(2);
	}

	init_raw_buffer();

	fd_serial = open_serial_port(global_conf.port, global_conf.speed);
	if ( fd_serial < 0 ) {
		exit(3);
	}

	pthread_create(&receiver_thread, NULL, serial_receiver, (void*)fd_serial);

	DBG("Main loop started.\n");
	while (terminate_flag == 0) {

		if ( get_raw_packet(&raw_packet, &packet_size) >= 0 ) {

			DBG("raw packet %d [%s]\n", packet_size, raw_packet);

			if ( cdr_packet_create(&packet, raw_packet, packet_size) == TRUE ) {
				//process_packet(fd_serial, &packet);

				if ( strcmp(packet.m_sDesc, "__HD") == 0 ) {


					ret = make_response_packet(buf, sizeof(buf)-1, MSG_ACK, &packet);
					printf("Make response [%s], ret= %d\n", buf, ret);
					dump_idx = 0;

				} else {
					ret = make_response_packet(buf, sizeof(buf)-1, MSG_ACK, &packet);
					printf("response [%s], ret= %d\n", buf, ret);
				}
				strcpy(dump_packet[dump_idx], raw_packet);
				write(fd_serial, buf, ret);
				cdr_packet_free(&packet);
				dump_idx++;

			}
		} 
	}
	DBG("Main loop end.\n");

	pthread_join( receiver_thread, NULL);

	close_serial_port();
	printf("program end\n");


	{
		int i;

		printf("packet raw data ==============\n");
		for ( i = 0 ; i < dump_idx; i++) {
			printf("#%d = { ", i);
			ret = 0;
			while(dump_packet[i][ret] ) {
				printf("0x%02x, ", dump_packet[i][ret]);
				ret ++;
			}
			printf("};\n");
		}
	}
	return 0;
}
#endif
