/*
 * Stolen from Cmus
 * Stolen from Linux 2.6.7
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEUS_LIST_H
#define DEUS_LIST_H

#include "./compiler.h" /* container_of */

static inline void prefetch(const void *x)
{
}

/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define DEUS_LIST_POISON1  ((void *) 0x00100100)
#define DEUS_LIST_POISON2  ((void *) 0x00200200)
#define DEUS_LIST_EMPTY ((struct deus_list_head) {NULL, NULL})

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("_xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct deus_list_head {
	struct deus_list_head *next, *prev;
};

#define DEUS_LIST_INIT(name) { &(name), &(name) }
#define DEUS_LIST(name) \ struct deus_list_head name = DEUS_LIST_INIT(name)

static inline void deus_list_init(struct deus_list_head *head)
{
	head->next = head;
	head->prev = head;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void _deus_list_add(struct deus_list_head *new,
			      struct deus_list_head *prev,
			      struct deus_list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline struct deus_list_head *deus_list_prev(struct deus_list_head *list)
{
	return list->prev;
}

static inline struct deus_list_head *deus_list_next(struct deus_list_head *list)
{
	return list->next;
}

/**
 * deus_list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void deus_list_add(struct deus_list_head *_new, struct deus_list_head *head)
{
	_deus_list_add(_new, head, head->next);
}

/**
 * deus_list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void deus_list_add_tail(struct deus_list_head *new, struct deus_list_head *head)
{
	_deus_list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void _deus_list_del(struct deus_list_head *prev, struct deus_list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * deus_list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: deus_list_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void deus_list_del(struct deus_list_head *entry)
{
	_deus_list_del(entry->prev, entry->next);
	entry->next = DEUS_LIST_POISON1;
	entry->prev = DEUS_LIST_POISON2;
}

/**
 * deus_list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void deus_list_del_init(struct deus_list_head *entry)
{
	_deus_list_del(entry->prev, entry->next);
	deus_list_init(entry);
}

/**
 * deus_list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void deus_list_move(struct deus_list_head *list, struct deus_list_head *head)
{
	_deus_list_del(list->prev, list->next);
	deus_list_add(list, head);
}

/**
 * deus_list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void deus_list_move_tail(struct deus_list_head *list,
				  struct deus_list_head *head)
{
	_deus_list_del(list->prev, list->next);
	deus_list_add_tail(list, head);
}

/**
 * deus_list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int deus_list_empty(const struct deus_list_head *head)
{
	return head->next == head;
}

static inline void _deus_list_splice(struct deus_list_head *list,
				 struct deus_list_head *head)
{
	struct deus_list_head *first = list->next;
	struct deus_list_head *last = list->prev;
	struct deus_list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/**
 * deus_list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void deus_list_splice(struct deus_list_head *list, struct deus_list_head *head)
{
	if (!deus_list_empty(list))
		_deus_list_splice(list, head);
}

/**
 * deus_list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void deus_list_splice_init(struct deus_list_head *list,
				    struct deus_list_head *head)
{
	if (!deus_list_empty(list)) {
		_deus_list_splice(list, head);
		deus_list_init(list);
	}
}

/**
 * deus_list_entry - get the struct for this entry
 * @ptr:	the &struct deus_list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the deus_list_struct within the struct.
 */
#define deus_list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * deus_list_for_each	-	iterate over a list
 * @pos:	the &struct deus_list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define deus_list_for_each(pos, head) \
	for (pos = (head)->next, prefetch(pos->next); pos != (head); \
		pos = pos->next, prefetch(pos->next))

/**
 * _deus_list_for_each	-	iterate over a list
 * @pos:	the &struct deus_list_head to use as a loop counter.
 * @head:	the head for your list.
 *
 * This variant differs from deus_list_for_each() in that it's the
 * simplest possible list iteration code, no prefetching is done.
 * Use this for code that knows the list to be very short (empty
 * or 1 entry) most of the time.
 */
#define _deus_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * deus_list_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct deus_list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define deus_list_for_each_prev(pos, head) \
	for (pos = (head)->prev, prefetch(pos->prev); pos != (head); \
		pos = pos->prev, prefetch(pos->prev))

/**
 * deus_list_for_each_safe	-	iterate over a list safe against removal of list entry
 * @pos:	the &struct deus_list_head to use as a loop counter.
 * @n:		another &struct deus_list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define deus_list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * deus_list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the deus_list_struct within the struct.
 */
#define deus_list_for_each_entry(pos, head, member)				\
	for (pos = deus_list_entry((head)->next, __typeof__(*pos), member),	\
		     prefetch(pos->member.next);			\
	     &pos->member != (head); 					\
	     pos = deus_list_entry(pos->member.next, __typeof__(*pos), member),	\
		     prefetch(pos->member.next))

/**
 * deus_list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the deus_list_struct within the struct.
 */
#define deus_list_for_each_entry_reverse(pos, head, member)			\
	for (pos = deus_list_entry((head)->prev, __typeof__(*pos), member),	\
		     prefetch(pos->member.prev);			\
	     &pos->member != (head); 					\
	     pos = deus_list_entry(pos->member.prev, __typeof__(*pos), member),	\
		     prefetch(pos->member.prev))

/**
 * deus_list_prepare_entry - prepare a pos entry for use as a start point in
 *			deus_list_for_each_entry_continue
 * @pos:	the type * to use as a start point
 * @head:	the head of the list
 * @member:	the name of the deus_list_struct within the struct.
 */
#define deus_list_prepare_entry(pos, head, member) \
	((pos) ? : deus_list_entry(head, __typeof__(*pos), member))

/**
 * deus_list_for_each_entry_continue -	iterate over list of given type
 *			continuing after existing point
 * @pos:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the deus_list_struct within the struct.
 */
#define deus_list_for_each_entry_continue(pos, head, member) 		\
	for (pos = deus_list_entry(pos->member.next, __typeof__(*pos), member),	\
		     prefetch(pos->member.next);			\
	     &pos->member != (head);					\
	     pos = deus_list_entry(pos->member.next, __typeof__(*pos), member),	\
		     prefetch(pos->member.next))

/**
 * deus_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop counter.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the deus_list_struct within the struct.
 */
#define deus_list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = deus_list_entry((head)->next, __typeof__(*pos), member),	\
		n = deus_list_entry(pos->member.next, __typeof__(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = deus_list_entry(n->member.next, __typeof__(*n), member))

#endif