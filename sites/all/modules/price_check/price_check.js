function openWithoutReferrer(url) {
  var site = window.open("", "hide_referrer");
  site.document.open();
  site.document.writeln('<script type="text/javascript">window.location = "' + url + '";</script>');
  site.document.close();
}

/*
// Open links with "hide-referrer" class without sending the referrer
$(document).on('click', 'a.hide-referrer', function(e) {
  e.preventDefault();
  openWithoutReferrer(e.target.href);
});
*/

// Open external links without sending the referrer
$(document).on('click', 'a', function(e) {
  href = e.target.getAttribute('href')
  if(href && href.hostname != window.location.hostname) {
    e.preventDefault();
    openWithoutReferrer(this.href);
  }  
});

(function($) {
Drupal.behaviors.myBehavior = {
  attach: function (context, settings) {
    $("form[id*='modify_item']").submit(function(e){
      var form = $(this);
      $.ajax({ 
           url   : form.attr('action'),
           type  : form.attr('method'),
           data  : form.serialize(), 
           success: function(response){
              alert(response);
           }
      });
      return false;
    });
  }
};
})(jQuery);