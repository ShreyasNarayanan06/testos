.global get_page_directory
.global set_page_directory
.global enable_paging
.global read_faulting_address
.global flush_TLB

get_page_directory:
    mov %cr3, %eax
    ret

set_page_directory:
    mov 4(%esp), %eax
    mov %eax, %cr3
    ret

enable_paging:
    mov %cr0, %eax
    or %eax, 0x80000000 
    mov %eax, %cr0        
    ret

read_faulting_address:
    mov %cr2, %eax
    ret

flush_TLB:
    movl  4(%esp), %eax
    invlpg (%eax)
    ret
    