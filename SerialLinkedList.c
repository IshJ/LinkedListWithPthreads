#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

//Linked list node definition
struct list_node_s {
    int data;
    struct list_node_s *next;
};

int Member(int value, struct list_node_s *head_p);

int Insert(int value, struct list_node_s **head_pp);

int Delete(int value, struct list_node_s **head_pp);

void getInputs(int argc, char *argv[]);

//n : initial number of elements in the linked list
//m :total number of operations on the list
int n, m;

//m_insert_fraction: fraction of total operations(m) which should be insertions
//m_delete_fraction: fraction of total operations(m) which should be deletions
//m_member_fraction: fraction of total operations(m) which should be member calls
float m_insert_fraction, m_delete_fraction, m_member_fraction;

int main(int argc, char *argv[]) {

    struct list_node_s *head = NULL;
    struct timeval startTime, endTime;
    double elapsedTime;
    int memberCount = 0, insertCount = 0, deleteCount = 0, totalCount = 0;

    getInputs(argc, argv);

    
    srand(time(NULL));
    //Initialization of the linked list
    for (int i = 0; i < n;) {
        int r = rand() % 65535;
        i += Insert(r, &head);
    }
    //calculations for the number of insertions, deletions, and member calls
    float m_insert = m_insert_fraction * m;
    float m_delete = m_delete_fraction * m;
    float m_member = m_member_fraction * m;

    FILE *fp;
    fp = fopen("serial.txt", "a");
    // Start time tracked
    gettimeofday(&startTime, NULL);
    while (totalCount < m) {
        int r = rand() % 65535;
        int rand_select = rand() % 3;

        if (rand_select == 0 && memberCount < m_member) {
            Member(r, head);
            memberCount++;
        }

        else if (rand_select == 1 && insertCount < m_insert) {
            Insert(r, &head);
            insertCount++;
        }

        else if (rand_select == 2 && deleteCount < m_delete) {
            Delete(r, &head);
            deleteCount++;
        }
        totalCount = memberCount + insertCount + deleteCount;
    }
    // Finish time tracked
    gettimeofday(&endTime, NULL);

    // Elapsed time tracked
    elapsedTime =
            (double) (endTime.tv_usec - startTime.tv_usec) / 1000000 + (double) (endTime.tv_sec - startTime.tv_sec);

    fprintf(fp, "%.6f\n", elapsedTime);

    fclose(fp);

    return 0;
}

int Member(int value, struct list_node_s *head_p) {
    struct list_node_s *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    }
    else {
        return 1;
    }
}

int Insert(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    }
    else {
        return 0;
    }
}

int Delete(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else {
        return 0;
    }
}

void getInputs(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Please give the command in this format : ./serialLinkedList <n> <m> <mMember> <mInsert> <mDelete>\n");
        exit(0);
    }

    n = (int) strtol(argv[1], (char **) NULL, 10);
    m = (int) strtol(argv[2], (char **) NULL, 10);

    m_member_fraction = (float) atof(argv[3]);
    m_insert_fraction = (float) atof(argv[4]);
    m_delete_fraction = (float) atof(argv[5]);


    //Validating the arguments
    if (n <= 0 || m <= 0 || m_member_fraction + m_insert_fraction + m_delete_fraction != 1.0) {
        printf("Please give the command with the arguments: ./serial_linked_list <n> <m> <mMember> <mInsert> <mDelete>\n");

        if (n <= 0)
            printf("Please provide a valid number of nodes for the linked list (value of n)\n");

        if (m <= 0)
            printf("Please provide a valid number of operations for the linked list (value of m)\n");

        if (m_member_fraction + m_insert_fraction + m_delete_fraction != 1.0)
            printf("Please provide valid fractions of operations for the linked list (value of mMember, mInsert, mDelete)\n");

        exit(0);
    }
}


