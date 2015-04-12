  <div id="header">
    <?php if ($page['header']) print render($page['header']);?>
  </div>

<div id="branding">
  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <h1 class="page-title"><?php print $title; ?></h1>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php print $breadcrumb; ?>
  <?php print render($primary_local_tasks); ?>
</div>
<div id="page">
  <div id="content">
    <div class="element-invisible"><a id="main-content"></a></div>
    <?php if ($messages): ?>
      <div id="console"><?php print $messages; ?></div>
    <?php endif; ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <?php if ($tabs): ?>
        <div class="tabs">
          <?php print render($tabs); ?>
        </div>
      <?php endif; ?>
      <div id="main">
        <?php print render($page['content']); ?>
      </div>
  </div>

  <div id="feed-icons">
    <?php print $feed_icons; ?>
  </div>

</div>
<?php if ($page['footer']):?>
  <div id="footer">
    <?php print render($page['footer']);?>
  </div>
<?php endif;?>
</div>
