
# Table of Contents

1.  [Implement FlexSC in xv6](#org1fe39e8)
    1.  [Design Report (Deadline: 10-Nov 2022 at 23:59 hrs)](#orgad3894c)
    2.  [Implementation (Deadline: 18-Nov 2022 at 23:59 hrs)](#orgd61f8f1)


<a id="org1fe39e8"></a>

# Implement FlexSC in xv6


<a id="orgad3894c"></a>

## Design Report (Deadline: 10-Nov 2022 at 23:59 hrs)

1.  Go through the paper `flexsc.pdf` and write a detailed report on how you
    will implement FlexSC (outlined in the paper) into `xv6`. Your report
    (`lab6/report/report.md`) should contain details on the following:
    -   A list of changes that you will make to `xv6`.
        -   The order of items on your list should reflect the order in which you
            will go about implementing the suggested changes.
        
        -   Each item on the list should explicitly mention if the item is a
            feature-add or a minor tweak or change to the existing `xv6` code. For
            example, adding a new system call (`flexsc_wait()`) is a minor tweak,
            but implementing its syscall handler function would be a new feature.
            Similarly, changing page table mappings to support `syscall pages` will
            be a minor tweak, but the actual reading/writing mechanism, allocation,
            managing status of syscalls, etc will be a new feature-add.
        
        -   For each feature-add, outline the envisaged design as well (algorithm
            you plan to implement, data structures you wish to maintain, etc).
    
    -   Mention your strategy of creating a user-level thread library that is
        needed to seamlessly support FlexSC system call from user applications.
        If you are planning to use an existing implementation (found elsewhere on
        the internet), mention the source and any tweaks that you have to do that
        source.
    
    -   Mention the test cases that you plan to create to test (and prove) that
        your implementation is working correctly.


<a id="orgd61f8f1"></a>

## Implementation (Deadline: 18-Nov 2022 at 23:59 hrs)

1.  Implement the changes mentioned in your report into `xv6` and demonstrate
    its working.
    
    NOTE: Any changes that are not mentioned in the report submitted earlier
    will not be graded. Minor tweaks to add corner cases that you missed in
    your design (but discovered during implementation) is allowed, but it
    should be documented clearly (bug found, and the change required to fix
    it).

