#include <stdlib.h>
#include <stdio.h>

int wait_user(int exit_condition) {
	printf("\nPress Enter to continue...");
	getchar();

	return exit_condition;
}

int main(int argc, char const *argv[]) {
	int renamed = rename("Settlers6.exe", "Settlers6.exe.bak");

	if (renamed != 0) {
		perror("Error: Creating backup of Settlers6.exe ");
		fprintf(stderr, "Make sure you're running this on the same directory of the executable.\n");
		return wait_user(EXIT_FAILURE);
	}

	FILE* original_exe = fopen("Settlers6.exe.bak", "rb");
	FILE* new_exe = fopen("Settlers6.exe", "wb");

	if (original_exe == NULL) {
		perror("Error: Failed to read Settlers6.exe.bak file ");
		fprintf(stderr, "Make sure you're running this on the same directory of the executable.\n");

		int renamed = rename("Settlers6.exe.bak", "Settlers6.exe");

		if (renamed != 0) {
			perror("Error: Reverting backup of Settlers6.exe.bak ");
			return wait_user(EXIT_FAILURE);
		}

		return wait_user(EXIT_FAILURE);
	}

	if (new_exe == NULL) {
		fclose(original_exe);

		perror("Error: Could not patch Settlers6.exe file ");
		fprintf(stderr, "Try running as administrator.\n");

		return wait_user(EXIT_FAILURE);
	}

	unsigned short i = 0;
	size_t read, written;
	unsigned char buffer[1291];

	do {
		read = fread(buffer, 1, sizeof buffer, original_exe);

		if (1291 * i++ == 0xBA97) {
			buffer[0] = 0xF9;
		}

		if (read) {
			written = fwrite(buffer, 1, read, new_exe);
		}
		else {
			written = 0;
		}
	} while ((read > 0) && (read == written));

	if (written) {
		perror("Error: The file was corrupted during patching ");

		int removed = remove("Settlers6.exe");

		if (removed != 0) {
			perror("Error: Reverting backup of Settlers6.exe.bak ");
			return wait_user(EXIT_FAILURE);
		}

		int renamed = rename("Settlers6.exe.bak", "Settlers6.exe");

		if (renamed != 0) {
			perror("Error: Reverting backup of Settlers6.exe.bak ");
			return wait_user(EXIT_FAILURE);
		}

		printf("The executable was reverted from backup\n");

		return wait_user(EXIT_FAILURE);
	}

	fclose(original_exe);
	fclose(new_exe);

	printf("Settlers 6 executable was patched successfully.\n");

	return wait_user(EXIT_SUCCESS);
}
