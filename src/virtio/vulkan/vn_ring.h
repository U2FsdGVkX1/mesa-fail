/*
 * Copyright 2021 Google LLC
 * SPDX-License-Identifier: MIT
 */

#ifndef VN_RING_H
#define VN_RING_H

#include "vn_common.h"

/**
 * A ring is a single-producer and single-consumer circular buffer.  The data
 * in the buffer are produced and consumed in order.  An externally-defined
 * mechanism is required for ring setup and notifications in both directions.
 * Notifications for new data from the producer are needed only when the
 * consumer is not actively polling, which is indicated by the ring status.
 *
 * For venus, the data are plain venus commands.  When a venus command is
 * consumed from the ring's perspective, there can still be ongoing CPU and/or
 * GPU works.  This is not an issue when the works generated by following
 * venus commands are correctly queued after the ongoing works.  There are
 * also venus commands that facilitate polling or waiting for ongoing works.
 */

/* the layout of a ring in a shmem */
struct vn_ring_layout {
   size_t head_offset;
   size_t tail_offset;
   size_t status_offset;

   size_t buffer_offset;
   size_t buffer_size;

   size_t extra_offset;
   size_t extra_size;

   size_t shmem_size;
};

static_assert(ATOMIC_INT_LOCK_FREE == 2 && sizeof(atomic_uint) == 4,
              "vn_ring_shared requires lock-free 32-bit atomic_uint");

/* pointers to a ring in a BO */
struct vn_ring_shared {
   const volatile atomic_uint *head;
   volatile atomic_uint *tail;
   volatile atomic_uint *status;
   void *buffer;
   void *extra;
};

struct vn_ring_submit {
   uint32_t seqno;

   struct list_head head;

   /* BOs to keep alive (TODO make sure shmems are pinned) */
   uint32_t shmem_count;
   struct vn_renderer_shmem *shmems[];
};

struct vn_ring {
   struct vn_renderer *renderer;

   uint32_t buffer_size;
   uint32_t buffer_mask;

   struct vn_ring_shared shared;
   uint32_t cur;

   struct list_head submits;
   struct list_head free_submits;

   /* Only one "waiting" thread may fulfill the "monitor" role at a time.
    * Every "report_period_us" or longer, the waiting "monitor" thread tests
    * the ring's ALIVE status, updates the "alive" atomic, and resets the
    * ALIVE status for the next cycle. Waiting non-"monitor" threads, just
    * check the "alive" atomic. The "monitor" role may be released and
    * acquired by another waiting thread dynamically.
    */
   struct {
      mtx_t mutex;
      atomic_int threadid;
      atomic_bool alive;

      /* constant and non-zero after ring init, if monitoring is enabled */
      uint32_t report_period_us;
   } monitor;
};

void
vn_ring_get_layout(size_t buf_size,
                   size_t extra_size,
                   struct vn_ring_layout *layout);

void
vn_ring_init(struct vn_ring *ring,
             struct vn_renderer *renderer,
             const struct vn_ring_layout *layout,
             void *shared);

void
vn_ring_fini(struct vn_ring *ring);

struct vn_ring_submit *
vn_ring_get_submit(struct vn_ring *ring, uint32_t shmem_count);

uint32_t
vn_ring_load_status(const struct vn_ring *ring);

void
vn_ring_unset_status_bits(struct vn_ring *ring, uint32_t mask);

bool
vn_ring_submit(struct vn_ring *ring,
               struct vn_ring_submit *submit,
               const struct vn_cs_encoder *cs,
               uint32_t *seqno);

void
vn_ring_wait(struct vn_ring *ring, uint32_t seqno);

#endif /* VN_RING_H */