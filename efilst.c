#include <stdio.h>
#include <efivar/efivar.h>
#include <errno.h>
#include <stdlib.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

int main(void)
{
    if (!efi_variables_supported())
    {
        perror("EFI variables are not supported.\n");
        return 1;
    }

    printf("EFI variables are supported.\n");
    printf("Listing all EFI variables:\n");
    printf("--------------------------\n\n");

    efi_guid_t *guid = NULL;
    char *name = NULL;

    // List all EFI variables
    while (efi_get_next_variable_name(&guid, &name) > 0)
    {
        char guidstr[256];
        char *pguidstr = guidstr;
        uint32_t attributes;
        size_t data_size = 0;
        uint8_t *data = NULL;

        // Convert GUID to string
        if (efi_guid_to_str(guid, &pguidstr) >= 0)
        {
            printf(ANSI_COLOR_RED "Variable: " ANSI_COLOR_RESET "%s\n", name);
            printf("GUID: %s\n", guidstr);

            // Get variable data
            if (efi_get_variable(*guid, name, &data, &data_size, &attributes) >= 0)
            {
                printf(ANSI_COLOR_GREEN "Value (hex): " ANSI_COLOR_RESET);
                for (size_t i = 0; i < data_size; i++)
                {
                    printf("%02x ", data[i]);
                }
                printf("\n");

                printf(ANSI_COLOR_BLUE "Value (ASCII): " ANSI_COLOR_RESET);
                for (size_t i = 0; i < data_size; i++)
                {
                    if (data[i] >= 32 && data[i] <= 126)
                    {
                        printf("%c", data[i]);
                    }
                    else
                    {
                        printf(".");
                    }
                }
                printf("\n");

                free(data);
            }
            else
            {
                printf("Error reading variable value\n");
            }

            printf("----------------------------------------------\n");
        }
    }

    return 0;
}