<?php

/**
 * @file
 * Default theme implementation for course outline block.
 *
 * Available variables:
 * - $course: The course entity with all properties
 * - $sessions: List of all session links.
 *
 * @see template_preprocess_course_outline()
 *
 * @ingroup themeable
 */
?>
<div class="course-outline">
  <h3> <?php print t('Course Outline'); ?> </h3>
  <?php print $sessions; ?>
</div>
